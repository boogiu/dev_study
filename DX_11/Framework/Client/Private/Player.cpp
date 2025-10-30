#include "Client_Defines.h"
#include "Player.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "ITileService.h"
#include "ICameraService.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "Animator3D.h"
#include "ObjectContainer.h"

#include "Target_Camera.h"
#include "Camera.h"

#include "PlayerStateMachine.h"
#include "ToolItem.h"
#include "HairParts.h"
#include "ClothParts.h"
#include "PlayerPart_Hand.h"
#include "AABB_Collider.h"

CPlayer::CPlayer()
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	:CGameObject(rhs)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	HRESULT hr = Add_Component<CSkeletalModel>()->Link_Model("GamePlay_Level", "PlayerBody.model");
	hr = Add_Component<CMaterial>()->Link_Material("GamePlay_Level", "PlayerBody.mat");
	Add_Component<CAnimator3D>();
	Add_Component<CObjectContainer>();
	Add_Component<CAABB_Collider>();

	m_InstanceTag = "Player";
	return hr;
}

HRESULT CPlayer::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);


	Add_AnimationClips();
	Add_PartObjects();

	m_pStateMachine = CPlayerStateMachine::Create(this);
	Get_Component<CSkeletalModel>()->SetDrawable(5, false);
	Get_Component<CSkeletalModel>()->SetDrawable(8, false);
	Get_Component<CSkeletalModel>()->SetDrawable(10, false);


	CMaterialInstance* SkinInstance = Get_Component<CMaterial>()->Find_MaterialByName("mSkin");
	SkinInstance->Override_Pass("SkinShader");

	CMaterialInstance* EyeInstance = Get_Component<CMaterial>()->Find_MaterialByName("mEye");
	EyeInstance->Override_Pass("EyeShader");

	CMaterialInstance* MouthInstance = Get_Component<CMaterial>()->Find_MaterialByName("mMouth");
	MouthInstance->Override_Pass("MouthShader");

	CMaterialInstance* CheekInstance = Get_Component<CMaterial>()->Find_MaterialByName("mCheek");
	CheekInstance->Override_Pass("CheekShader");

	for (auto& instance : Get_Component<CMaterial>()->Get_Material_Instance()) {
		instance->Get_MaterialData()->Link_Shader("GamePlay_Level", "PlayerShader.hlsl");
	}

	Get_Component<CAABB_Collider>()->Make_MinMaxCollider(
		{ { -2,0,-2 }, {2,5,2} }
	);
	return S_OK;
}

void CPlayer::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
	Update_Input(dt);
}

void CPlayer::Update(_float dt)
{
	Update_TileInfo(dt); 
	Update_Movement(dt);
	m_pStateMachine->Update(dt);
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CPlayer::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CPlayer::Render_GUI()
{
	__super::Render_GUI();
	m_pStateMachine->Render_State(this);
	ImGui::Begin("Item Control");

	if (ImGui::Button("None")) {
		ITEM_DATA_DESC Data = {};
		Data.eType = ITEM_TYPE::NONE;
		Data.materialName = "";
		Data.modelName = "";
		Data.TypeTag = "None";
		Change_Item(Data);
	}

	if (ImGui::Button("Axe")) {
		ITEM_DATA_DESC Data = {};
		Data.eType = ITEM_TYPE::AXE;
		Data.materialName = "ToolAxeFirst.mat";
		Data.modelName = "ToolAxeFirst.model";
		Data.TypeTag = "Axe";
		Change_Item(Data);
	}

	if (ImGui::Button("Net")) {
		ITEM_DATA_DESC Data = {};
		Data.eType = ITEM_TYPE::NET;
		Data.materialName = "ToolNetFirst.mat";
		Data.modelName = "ToolNetFirst.model";
		Data.TypeTag = "Net";
		Change_Item(Data);
	}

	if (ImGui::Button("Scoop")) {
		ITEM_DATA_DESC Data = {};
		Data.eType = ITEM_TYPE::SCOOP;
		Data.materialName = "ToolScoopFirst.mat";
		Data.modelName = "ToolScoopFirst.model";
		Data.TypeTag = "Scoop";
		Change_Item(Data);
	}
	ImGui::End();

	ImGui::Begin("Control_Packet");
	ImGui::Checkbox("MsgMove", &m_ControlPack.MsgMove);
	ImGui::Checkbox("MsgDash", &m_ControlPack.MsgDash);
	ImGui::Checkbox("MsgAction", &m_ControlPack.MsgAction);
	ImGui::Checkbox("MsgToolUse", &m_ControlPack.MsgToolUse);
	ImGui::Checkbox("MsgInteraction", &m_ControlPack.MsgInteraction);
	ImGui::Checkbox("MsgUI", &m_ControlPack.MsgUI);
	ImGui::End();
}

void CPlayer::Update_Input(_float dt)
{
	auto pInpuDev = CGameInstance::GetInstance()->Get_InputDev();
	auto& control = m_ControlPack;
	control.Reset();

	_uint StateActionMask = m_pStateMachine->Get_CurrentMask(STATE_LAYER::ACTION);
	_uint StateToolMask = m_pStateMachine->Get_CurrentMask(STATE_LAYER::TOOL);

	auto AllowAction = [&](InputMask type) {return (StateActionMask & (1 << static_cast<_uint>(type))) != 0;};
	auto AllowTool = [&](InputMask type) {return (StateToolMask & (1 << static_cast<_uint>(type))) != 0;};

	if (AllowAction(InputMask::MOVE))
	{
		_float2 moveAxis = { 0.f, 0.f };
		if (pInpuDev->Key_Down(VK_UP))				
			moveAxis.y = -1.f;
		if (pInpuDev->Key_Down(VK_DOWN))		
			moveAxis.y = +1.f;
		if (pInpuDev->Key_Down(VK_LEFT))			
			moveAxis.x = +1.f;
		if (pInpuDev->Key_Down(VK_RIGHT))		
			moveAxis.x = -1.f;

		if (moveAxis.x != 0 || moveAxis.y != 0)
			control.MsgMove = true;
		else 
			control.MsgMove = false;

		if (pInpuDev->Key_Down(VK_SHIFT) && control.MsgMove)
			control.MsgDash = true;
		XMStoreFloat2(&m_MovementPack.vInputAxis, XMVector2Normalize(XMLoadFloat2(&moveAxis)));
	}

	if (AllowAction(InputMask::ACTION)) {
		if (pInpuDev->Key_Down(VK_SPACE))
			control.MsgAction = true;
	}
	if (AllowAction(InputMask::TOOL)) {
		if (pInpuDev->Key_Down(VK_SPACE))
				control.MsgToolUse = true;
	}

}

void CPlayer::Update_Movement(_float dt)
{
	auto pInput = CGameInstance::GetInstance()->Get_InputDev();

	_float2 moveAxis = m_MovementPack.vInputAxis;
	if (fabs(moveAxis.x) > 0.01f || fabs(moveAxis.y) > 0.01f)
		m_MovementPack.fTargetDegree = XMConvertToDegrees(atan2(moveAxis.x, moveAxis.y));

	_float DeltaDegree = m_MovementPack.fTargetDegree - m_MovementPack.fCurrentDegree;

	// -180~180 범위로 정규화
	while (DeltaDegree > 180.f) DeltaDegree -= 360.f;
	while (DeltaDegree < -180.f) DeltaDegree += 360.f;

	_float RotSpeed = dt * 480;

	if (fabs(DeltaDegree) > 150.f)
		m_MovementPack.bFliping = true;

	if (fabs(DeltaDegree) > 10.f) {
		if (m_MovementPack.bFliping)
			m_MovementPack.fCurrentDegree -= RotSpeed;
		else
			m_MovementPack.fCurrentDegree += (DeltaDegree > 0 ? RotSpeed : -RotSpeed);
	}

	else {
		m_MovementPack.bFliping = false;
		m_MovementPack.fCurrentDegree = m_MovementPack.fTargetDegree;
	}
}

void CPlayer::Update_TileInfo(_float dt)
{
	auto TileSys = CGameInstance::GetInstance()->Get_TileSystem();
	auto Info = TileSys->Get_TileSystemInfo();

	m_TileInfoPack.nowIndex = TileSys->Get_IndexByPosition(Get_Position());
	m_TileInfoPack.infos.resize(9);

	vector<TILE_INFO> worldInfo = {};
	_uint ValidIndex = TileSys->Get_NeighborInfoByIndex(m_TileInfoPack.nowIndex, worldInfo);
	/*현재 y축 회전값을 360으로 나머지 연산*/
	_float yaw = fmodf(m_MovementPack.fCurrentDegree, 360.f);
	/*음수면 보정*/
	if (yaw < 0.f)
		yaw += 360.f;

	_uint RotCount = static_cast<_uint>(yaw / 45.f) % 8;

	for (size_t i = 0; i < 9; i++)
	{
		NEIGHBOR_INDEX neighbor = Get_Negihbor(i);
		if ((ValidIndex & neighbor) != 0) {
			m_TileInfoPack.infos[Get_Index(Rotate45_CCW(neighbor, RotCount))] = worldInfo[Get_Index(neighbor)];
			m_TileInfoPack.neighboValidFlag |= static_cast<_uint>(Rotate45_CCW(neighbor, RotCount));
		}
	}
	Get_Index(NEIGHBOR_INDEX::UP);
}

void CPlayer::Adjust_To_Foward()
{
	//현재 룩벡터

	//바라봐야하는 벡터
	TILE_INDEX index = Get_FowardIndex();

	//사이각/

	//목표 각으로 전환
}


void CPlayer::Change_Item(ITEM_DATA_DESC desc)
{
	if (m_ItemPack.CurItem.modelName == desc.modelName) {
		return;
	}
	m_ItemPack.DstItem = desc;
	m_pStateMachine->Request_ChangeState(STATE_LAYER::ACTION,"Action_TransTool_State");
}

void CPlayer::Set_CurItemData(ITEM_DATA_DESC desc)
{
	if (m_ItemPack.CurItem.modelName == desc.modelName) {
		return;
	}

	m_ItemPack.CurItem = desc;
	m_ItemPack.DstItem = {};
	CGameObject* pObj = Get_Component<CObjectContainer>()->Find_ObjectByName("Right_Hand");
	dynamic_cast<CPlayerPart_Hand*>(pObj)->Change_Item(desc);
}

_bool CPlayer::Can_Walk(_float2& moveAxis)
{
	//대각선 블럭 대응 못하는 중
	if (m_ControlPack.MsgMove== false)
		return false;

	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	_float2 tmpAxis = moveAxis;
	_float4  NextPos =Get_Position();
	NextPos.x += tmpAxis.x;
	NextPos.z += tmpAxis.y;
	TILE_INDEX nextIndex = CGameInstance::GetInstance()->Get_TileSystem()->Get_IndexByPosition(NextPos);

	_uint Flag = tileSystem->Get_TileFlagByIndex(nextIndex);
	if ((CANT_WALK & Flag) == 0)
		return true;

	/*움직일 수 없음*/
	_bool blockX = false;
	_bool blockZ = false;

	/*X축 검사*/
	_float4 testX = Get_Position();
	testX.x += tmpAxis.x;
	TILE_INDEX testIdxX = tileSystem->Get_IndexByPosition(testX);
	if (tileSystem->Get_TileFlagByIndex(testIdxX) & static_cast<_uint>(CANT_WALK))
		blockX = true;

	/*Z축 검사*/
	_float4 testZ = Get_Position();
	testZ.z += tmpAxis.y;
	TILE_INDEX testIdxZ = tileSystem->Get_IndexByPosition(testZ);

	if (tileSystem->Get_TileFlagByIndex(testIdxZ) & static_cast<_uint>(CANT_WALK))
		blockZ = true;

	if (blockX)
	{
		tmpAxis.x = 0.f;
		tmpAxis.y -= 0.2f * (moveAxis.x > 0 ? -1 : 1);
	}
	if (blockZ) {
		tmpAxis.x -= 0.2f * (moveAxis.y < 0 ? -1 : 1);
		tmpAxis.y = 0.0f;
	}

	moveAxis = tmpAxis;

	if (tmpAxis.x != 0.f || tmpAxis.y != 0.f)
		return true;

	return false;
}

TILE_INDEX CPlayer::Get_FowardIndex()
{
	_float4 LookVec = {};
	XMStoreFloat4(&LookVec, m_pTransform->Dir(STATE::LOOK));
	auto TileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	TILE_INDEX index = TileSystem->Get_IndexByPosition(Get_Position());

	if (LookVec.x > -0.2f)
		index.IndexX += 1;
	if(LookVec.x < 0.2f)
		index.IndexX -= 1;

	if (LookVec.z> -0.2f)
		index.IndexZ += 1;
	if (LookVec.z < 0.2f)
		index.IndexZ -= 1;

	return index;
}

void CPlayer::ActiveCollider_Tool(_bool active, string Event)
{
	CGameObject* pHand = Get_Component<CObjectContainer>()->Find_ObjectByName("Right_Hand");
	CPlayerPart_Hand* pHandPart = dynamic_cast<CPlayerPart_Hand*>(pHand);
	pHandPart->Active_ColliderTool(active,Event);
}

void CPlayer::ActiveCollider_LeftHand(_bool active, string Event)
{
	CGameObject* pHand = Get_Component<CObjectContainer>()->Find_ObjectByName("Left_Hand");
	CPlayerPart_Hand* pHandPart = dynamic_cast<CPlayerPart_Hand*>(pHand);
	pHandPart->Active_ColliderHand(active, Event);
}

void CPlayer::Add_AnimationClips()
{
	Get_Component<CAnimator3D>()->LinkAnimate_Model("GamePlay_Level", "PlayerBody.model");

	/*움직임*/
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Base_Wait.anim", "Player", true);

	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Move_Run_F.anim", "Player", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Move_Dash_F.anim", "Player", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "MoveTurn_Run_L.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "MoveTurn_Dash_L.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToStop_RunLatter_L.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToStop_DashLatter_L.anim", "Player", false);

	/*툴 = Axe*/
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolAxe_Air.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolAxe_APose.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolAxe_Hit.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolAxe_Ready.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolAxe_ReadyKeep.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolAxe_Repelled.anim", "Player", false);

	/*툴 = NET*/
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolNet_APose.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolNet_APoseDash.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolNet_Get.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolNet_Slip.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolNet_Swing.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolNet_SwingStop_Ground.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolNet_SwingStop_Upper.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolNet_SwingStop_Lower.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolNet_SwingStop_Middle.anim", "Player", false);


	/*툴 = SCOOP*/
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolScoop_APose.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolScoop_Air.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolScoop_BuryHole.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolScoop_Dig.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolScoop_DigStump.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolScoop_Repelled.anim", "Player", false);

	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Tree_Shake.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Tree_ShakeReadyKeep.anim", "Player", true);

	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Base_EquipOff.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Base_EquipOn.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Pickup.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Pickup_Get.anim", "Player", false);
}

void CPlayer::Add_PartObjects()
{
	CPlayerPart_Hand::CHARACTER_PARTS_DESC* pRHandDesc = new CPlayerPart_Hand::CHARACTER_PARTS_DESC;
	pRHandDesc->pOwner = this;
	pRHandDesc->BoneName = "Armature_Hand_R";

	CPlayerPart_Hand::CHARACTER_PARTS_DESC* pLHandDesc = new CPlayerPart_Hand::CHARACTER_PARTS_DESC;
	pLHandDesc->pOwner = this;
	pLHandDesc->BoneName = "Armature_Hand_L";

	PLAYER_PARTS_DESC* pHairDesc = new PLAYER_PARTS_DESC;
	pHairDesc->pPlayer = this;

	PLAYER_PARTS_DESC* pHairCapDesc = new PLAYER_PARTS_DESC;
	pHairCapDesc->pPlayer = this;

	CClothParts::CLOTHES_DESC* pTopDesc = new CClothParts::CLOTHES_DESC;
	pTopDesc->pPlayer = this;
	pTopDesc->ClothType = "PlayerTopsTopYshirtsL";

	CClothParts::CLOTHES_DESC* pBottomDesc = new CClothParts::CLOTHES_DESC;
	pBottomDesc->pPlayer = this;
	pBottomDesc->ClothType = "PlayerBottomsPantsNormal";

	CGameObject* pRightHand = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_PlayerPart_Hand" })
		.Add_ObjDesc(pRHandDesc)
		.Build("Right_Hand");

	CGameObject* pLeftHand = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_PlayerPart_Hand" })
		.Add_ObjDesc(pLHandDesc)
		.Build("Left_Hand");

	CGameObject* pHair = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_HairParts" })
		.Add_ObjDesc(pHairDesc)
		.Build("Hair");

	CGameObject* pHairCap = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_HairCapParts" })
		.Add_ObjDesc(pHairCapDesc)
		.Build("HairCap");

	CGameObject* pTop = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_ClothParts" })
		.Add_ObjDesc(pTopDesc)
		.Build("Top");

	CGameObject* pBottom = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_ClothParts" })
		.Add_ObjDesc(pBottomDesc)
		.Build("Bottom");

	Get_Component<CObjectContainer>()->Add_Child(pRightHand, false);
	Get_Component<CObjectContainer>()->Add_Child(pLeftHand, false);
	Get_Component<CObjectContainer>()->Add_Child(pHair, false);
	Get_Component<CObjectContainer>()->Add_Child(pHairCap, false);
	Get_Component<CObjectContainer>()->Add_Child(pTop, true);
	Get_Component<CObjectContainer>()->Add_Child(pBottom, true);
}

CPlayer* CPlayer::Create()
{
	CPlayer* instance = new CPlayer();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CPlayer");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CPlayer::Clone(INIT_DESC* pArg)
{
	CPlayer* instance = new CPlayer(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CPlayer");
		Safe_Release(instance);
	}

	return instance;
}

void CPlayer::Free()
{
	__super::Free();
	Safe_Release(m_pStateMachine);
}