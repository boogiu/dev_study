#include "Client_Defines.h"
#include "Player.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "ITileService.h"
#include "ICameraService.h"
#include "IResourceService.h"
#include "ILevelService.h"

#include "Level.h"
#include "ItemSpawner.h"
#include "Insect_Object.h"
#include "UI_Responcer.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"
#include "MaterialAnimator.h"
#include "Texture.h"

#include "Animator3D.h"
#include "ObjectContainer.h"

#include "Target_Camera.h"
#include "Camera.h"

#include "PlayerStateMachine.h"
#include "ToolItem.h"
#include "HairParts.h"
#include "ClothParts.h"
#include "PlayerPart_Hand.h"
#include "Player_Inventory.h"
#include "AABB_Collider.h"
#include "OBB_Collider.h"

#include "Item_Object.h"
#include "InsectSpawner.h"
#include "EventSystem.h"
#include "NonPlayer.h"
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
	Add_Component<COBB_Collider>();
	Add_Component<CMaterialAnimator>();

	m_InstanceTag = "Player";
	return hr;
}

HRESULT CPlayer::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Add_AnimationClips();
	Add_PartObjects();
	Set_TargetCamera();
	Add_Inventory();

	m_pStateMachine = CPlayerStateMachine::Create(this);
	Get_Component<CSkeletalModel>()->SetDrawable(5, false);
	Get_Component<CSkeletalModel>()->SetDrawable(8, false);
	Get_Component<CSkeletalModel>()->SetDrawable(10, false);
	Get_Component<CSkeletalModel>()->ShadowCast(true);
	Get_Component<CMaterialAnimator>()->LinkAnimate_Material(Get_Component<CMaterial>());

	Add_MaterialAnim();

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

	Get_Component<CCollider>()->Make_MinMaxCollider(
		{ { -3,0,-5 }, {3,5,6} }
	);

	/*Debug*/
	auto ItemSpawner = CGameInstance::GetInstance()->Get_LevelMgr()->Get_CurrentLevel()->Get_LevelObject<CItemSpawner>();
	TOOL_DATA_DESC AxeData = ItemSpawner->Get_ItemData("ToolAxeFirst");
	TOOL_DATA_DESC NetData = ItemSpawner->Get_ItemData("ToolNetFirst");
	TOOL_DATA_DESC ScoopData = ItemSpawner->Get_ItemData("ToolScoopFirst");

	Add_ITEM(AxeData);
	Add_ITEM(NetData);
	Add_ITEM(ScoopData);

	Add_ITEM(ItemSpawner->Get_ItemData("UnitIconPltWood"));
	Add_ITEM(ItemSpawner->Get_ItemData("UnitIconPltWood"));
	Add_ITEM(ItemSpawner->Get_ItemData("UnitIconPltWood"));
	Add_ITEM(ItemSpawner->Get_ItemData("FtrWoodPile"));

	return S_OK;
}

void CPlayer::Awake()
{
	auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
	if (!nowLevel)
		return;
	auto EventSys = nowLevel->Get_LevelObject<CEventSystem>();
	EventSys->Add_Listner<CPlayer, BaseEvent>(this, &CPlayer::OnEventAction);
}

void CPlayer::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);

	Update_Input(dt);
}

void CPlayer::Update(_float dt)
{
	Update_Movement(dt);
	Update_TileInfo(dt);
	BroadCast_Position();
	Get_Component<CMaterialAnimator>()->Update_Animation(dt);
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

}

void CPlayer::Update_Input(_float dt)
{
	auto pInpuDev = CGameInstance::GetInstance()->Get_InputDev();
	auto& control = m_ControlPack;
	control.Reset();

	if (m_InfoPack.WorkBenchEncounter && pInpuDev->Key_Tap(VK_SPACE)) {
		Open_Craft();
	}

	if (control.MsgForceBlock)
		return;

	_uint StateActionMask = m_pStateMachine->Get_CurrentMask(STATE_LAYER::ACTION);
	auto AllowAction = [&](InputMask type) {return (StateActionMask & (1 << static_cast<_uint>(type))) != 0; };

	if (pInpuDev->Key_Down(VK_SHIFT))
		control.MsgAdd = true; /*대쉬*/

	/*해당 상태에서 해당 키입혁 마스크가 가능한가*/
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

		XMStoreFloat2(&m_MovementPack.vInputAxis, XMVector2Normalize(XMLoadFloat2(&moveAxis)));
	}

	if (AllowAction(InputMask::ACTION)) {
		if (pInpuDev->Key_Down(VK_SPACE))
			control.MsgAction = true;
	}

	if (AllowAction(InputMask::PICKUP)) {
		if (pInpuDev->Key_Tap(VK_CONTROL))
			control.MsgPickup = true;
	}

	if (AllowAction(InputMask::BAG)) {
		if (pInpuDev->Key_Tap('I'))
			control.MsgBag = true;
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

	_float RotSpeed = dt * 400;

	if (fabs(DeltaDegree) > 150.f)
		m_MovementPack.bFliping = true;

	if (fabs(DeltaDegree) > 6.f) {
		if (m_MovementPack.bFliping)
			m_MovementPack.fCurrentDegree -= RotSpeed;
		else
			m_MovementPack.fCurrentDegree += (DeltaDegree > 0 ? RotSpeed : -RotSpeed);
	}

	else {
		m_MovementPack.bFliping = false;
		m_MovementPack.fCurrentDegree = m_MovementPack.fTargetDegree;
	}
	auto TileSys = CGameInstance::GetInstance()->Get_TileSystem();
	m_MovementPack.fPlayerHeight = (TileSys->Get_TileHeightByPosition(Get_Position()) - Get_Position().y);
	m_pTransform->Override_Rotation({ 0,1,0,0 }, XMConvertToRadians(m_MovementPack.fCurrentDegree));

}

void CPlayer::Update_TileInfo(_float dt)
{
	auto TileSys = CGameInstance::GetInstance()->Get_TileSystem();
	auto Info = TileSys->Get_TileSystemInfo();

	m_TileInfoPack.Range_FowardInfo = TileSys->Get_InfoByIndex(Get_FowardIndex());
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
	TILE_INDEX currIndex = TileSys->Get_IndexByPosition(Get_Position());

	if (false == m_TileInfoPack.nowIndex.isSame(currIndex)) {
		m_vPrevIndex = m_TileInfoPack.nowIndex;
		m_TileInfoPack.nowIndex = currIndex;
		TileSys->Add_TileFlagByIndex(m_TileInfoPack.nowIndex, static_cast<_uint>(TILE_FLAG::ONPLAYER));
		TileSys->Remove_TileFlagByIndex(m_vPrevIndex, static_cast<_uint>(TILE_FLAG::ONPLAYER));

	}
}

void CPlayer::Adjust_To(_fvector pos)
{
	// 현재 룩 벡터
	_vector vLook = m_pTransform->Dir(STATE::LOOK);
	vLook = XMVector3Normalize(vLook);

	// 현재 위치
	_float4 vNowPos = Get_Position();

	_vector vTargetDir = pos - XMLoadFloat4(&vNowPos);
	vTargetDir = XMVector3Normalize(vTargetDir);
	vLook = XMVectorSetY(vLook, 0.f);
	vTargetDir = XMVectorSetY(vTargetDir, 0.f);

	// 방향 각도 계산 (Y축 기준 평면 상)
	_float angle =
		atan2(
			XMVectorGetX(vTargetDir),  // x 성분
			XMVectorGetZ(vTargetDir)   // z 성분
		) -
		atan2(

			XMVectorGetX(vLook),
			XMVectorGetZ(vLook)
		);

	if (angle > XM_PI) angle -= XM_2PI;
	if (angle < -XM_PI) angle += XM_2PI;

	m_MovementPack.fTargetDegree = m_MovementPack.fCurrentDegree + XMConvertToDegrees(angle);
}

void CPlayer::Adjust_To_Foward()
{
	// 현재 룩 벡터
	_vector vLook = m_pTransform->Dir(STATE::LOOK);
	vLook = XMVector3Normalize(vLook);

	// 바라봐야 하는 타일의 중심 위치
	TILE_INDEX forwardIndex = Get_FowardIndex();
	auto TileSys = CGameInstance::GetInstance()->Get_TileSystem();

	_float4 vDestPos = TileSys->Get_PositionByIndex(forwardIndex, ANCHOR::Center);

	// 현재 위치
	_float4 vNowPos = Get_Position();

	_vector vTargetDir = XMLoadFloat4(&vDestPos) - XMLoadFloat4(&vNowPos);
	vTargetDir = XMVector3Normalize(vTargetDir);
	vLook = XMVectorSetY(vLook, 0.f);
	vTargetDir = XMVectorSetY(vTargetDir, 0.f);

	// 방향 각도 계산 (Y축 기준 평면 상)
	_float angle =
		atan2(
			XMVectorGetX(vTargetDir),  // x 성분
			XMVectorGetZ(vTargetDir)   // z 성분
		) -
		atan2(

			XMVectorGetX(vLook),
			XMVectorGetZ(vLook)
		);

	if (angle > XM_PI) angle -= XM_2PI;
	if (angle < -XM_PI) angle += XM_2PI;

	m_MovementPack.fTargetDegree = m_MovementPack.fCurrentDegree + XMConvertToDegrees(angle);
}

void CPlayer::Adjust_To_WorldFoward()
{
	m_MovementPack.fTargetDegree = 0;
}

void CPlayer::OnCollisionEnter(COLLISION_CONTEXT context)
{
	m_InfoPack.pEncounter = context.Owner;
	m_InfoPack.EncounterTag = context.Owner->Get_Tag();

	if (context.EventTag == "PickedByHand")
	{
		m_pInventory->Add_ItemToInventory(dynamic_cast<CItem_Object*>(context.Owner)->Get_ItemData());
	}

	if (context.Owner->Has_Tag("WorkBench")) {
		m_InfoPack.WorkBenchEncounter = true;
	}

	m_pStateMachine->OnCollisionEnter(context);
}

void CPlayer::OnCollisionStay(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("NPC")) {
		if (nullptr == m_InfoPack.pEncounterNpc) {
			if (CNonPlayer* pNpc = dynamic_cast<CNonPlayer*>(context.Owner))
				m_InfoPack.pEncounterNpc = pNpc;
		}
	}
	m_pStateMachine->OnCollisionStay(context);
}

void CPlayer::OnCollisionExit(COLLISION_CONTEXT context)
{
	m_pStateMachine->OnCollisionExit(context);

	if (context.Owner->Has_Tag("NPC")) {
		m_InfoPack.pEncounterNpc = nullptr;
	}

	if (context.Owner->Has_Tag("WorkBench")) {
		m_InfoPack.WorkBenchEncounter = false;
	}
}

void CPlayer::Camera_Zoom_In(CGameObject* subject)
{
	if (!subject)
		m_pCamera->Execute_ZoomIn();
	else {
		m_pCamera->Execute_Talking(subject);
	}
}
void CPlayer::Camera_Zoom_Out(CGameObject* subject)
{
	m_pCamera->Release_ZoomIn();
}
void CPlayer::Open_Inventory()
{
	m_pInventory->Open_Inventory();
}
void CPlayer::Close_Inventory()
{
	m_pInventory->Close_Inventory();
}
void CPlayer::Change_Item(TOOL_DATA_DESC desc)
{
	if (m_ItemPack.CurItem.modelName == desc.modelName) {
		return;
	}
	m_ItemPack.DstItem = desc;
	m_pStateMachine->Request_ChangeState(STATE_LAYER::ACTION, "Action_TransTool_State");
}

void CPlayer::Set_CurItemData(TOOL_DATA_DESC desc)
{
	if (m_ItemPack.CurItem.modelName == desc.modelName) {
		return;
	}

	m_ItemPack.CurItem = desc;
	m_ItemPack.DstItem = {};
	CGameObject* pObj = Get_Component<CObjectContainer>()->Find_ObjectByName("Right_Hand");
	dynamic_cast<CPlayerPart_Hand*>(pObj)->Change_Item(desc);
}

HRESULT CPlayer::Add_ITEM(ITEM_DATA_DESC item)
{
	return m_pInventory->Add_ItemToInventory(item);
}

HRESULT CPlayer::Set_InvenEvent(ITEM_DATA_DESC item, _int Slot, wstring SelectedEvent)
{
	auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
	auto Spawner = nowLevel->Get_LevelObject<CItemSpawner>();
	auto Insect = nowLevel->Get_LevelObject<CInsectSpawner>();
	if (SelectedEvent == L"근처에 두기")
	{
		m_pInventory->PullOut_Item(Slot);
		Spawner->ThrowItem(item, m_pTransform->Get_Pos(), m_pTransform->Dir(STATE::LOOK));
	}

	else if (SelectedEvent == L"1개 꺼내기") {
		m_pInventory->PullOut_ToOtherSlot(Slot);
	}

	else if (SelectedEvent == L"1개 먹기") {
		m_InfoPack.pObjectOnLeftHand = Spawner->SpawnItem(item.FileName);
		m_pStateMachine->Request_ChangeState(STATE_LAYER::ACTION, "Action_Eat_State");
		m_pInventory->PullOut_Item(Slot);
	}
	else if (SelectedEvent == L"들기") {
		m_ControlPack.MsgBag = false;
		Change_Item(item);
	}
	else if (SelectedEvent == L"장착 해제하기") {
		m_ControlPack.MsgBag = false;
		Change_Item(ITEM_DATA_DESC{});
	}
	else if (SelectedEvent == L"풀어주기") {
		m_pInventory->PullOut_Item(Slot);
		Insect->SpawnInsect(item.Additionaldata, Get_Position());
	}
	return S_OK;
}

void CPlayer::Open_EventMsg(EventMsgDesc* evtMsg)
{
	auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
	auto UI_Responder = nowLevel->Get_LevelObject<CUI_Responcer>();
	UI_Responder->Active_UI("EvtMsg", evtMsg);
}

void CPlayer::BroadCast_Talk(OnStartDialogue evt)
{
	auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
	auto evtSys = nowLevel->Get_LevelObject<CEventSystem>();

	evtSys->OnBroadCast<BaseEvent>(evt);
}

_bool CPlayer::Can_Walk(_float2& moveAxis)
{
	//대각선 블럭 대응 못하는 중
	if (m_ControlPack.MsgMove == false)
		return false;

	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	_float2 tmpAxis = moveAxis;
	_float4  NextPos = Get_Position();
	NextPos.x += tmpAxis.x;
	NextPos.z += tmpAxis.y;
	TILE_INDEX nextIndex = CGameInstance::GetInstance()->Get_TileSystem()->Get_IndexByPosition(NextPos);

	_uint Flag = tileSystem->Get_TileFlagByIndex(nextIndex);

	if ((Flag & (CANT_WALK | TILE_FLAG::ONCHARACTER)) == 0)
		return true;

	/*움직일 수 없음*/
	_bool blockX = false;
	_bool blockZ = false;

	/*X축 검사*/
	_float4 testX = Get_Position();
	testX.x += tmpAxis.x;
	TILE_INDEX testIdxX = tileSystem->Get_IndexByPosition(testX);
	if (tileSystem->Get_TileFlagByIndex(testIdxX) & static_cast<_uint>((CANT_WALK | TILE_FLAG::ONCHARACTER)))
		blockX = true;

	/*Z축 검사*/
	_float4 testZ = Get_Position();
	testZ.z += tmpAxis.y;
	TILE_INDEX testIdxZ = tileSystem->Get_IndexByPosition(testZ);

	if (tileSystem->Get_TileFlagByIndex(testIdxZ) & static_cast<_uint>((CANT_WALK | TILE_FLAG::ONCHARACTER)))
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

_bool CPlayer::Can_Talk()
{
	return !(m_InfoPack.isTalking || m_InfoPack.isCrafting);
}

TILE_INDEX CPlayer::Get_FowardIndex()
{
	_vector Look = XMVector4Normalize(m_pTransform->Dir(STATE::LOOK));
	Look *= m_fRange;

	_float4 myPos = Get_Position();
	_float4 DstPos;

	XMStoreFloat4(&DstPos, XMLoadFloat4(&myPos) + Look);
	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();

	return tileSystem->Get_IndexByPosition(DstPos);
}

void CPlayer::ActiveCollider_Tool(_bool active, string Event)
{
	CGameObject* pHand = Get_Component<CObjectContainer>()->Find_ObjectByName("Right_Hand");
	CPlayerPart_Hand* pHandPart = dynamic_cast<CPlayerPart_Hand*>(pHand);
	pHandPart->Active_ColliderTool(active, Event);
}

void CPlayer::ActiveCollider_LeftHand(_bool active, string Event)
{
	CGameObject* pHand = Get_Component<CObjectContainer>()->Find_ObjectByName("Left_Hand");
	CPlayerPart_Hand* pHandPart = dynamic_cast<CPlayerPart_Hand*>(pHand);
	pHandPart->Active_ColliderHand(active, Event);
}

void CPlayer::ActiveCollider_RightHand(_bool active, string Event)
{
	CGameObject* pHand = Get_Component<CObjectContainer>()->Find_ObjectByName("Right_Hand");
	CPlayerPart_Hand* pHandPart = dynamic_cast<CPlayerPart_Hand*>(pHand);
	pHandPart->Active_ColliderHand(active, Event);
}

CLevel* CPlayer::Get_NowLevel()
{
	return CGameInstance::GetInstance()->Get_LevelMgr()->Get_CurrentLevel();
}

void CPlayer::Add_AnimationClips()
{
	auto Animator = Get_Component<CAnimator3D>();
	Animator->LinkAnimate_Model("GamePlay_Level", "PlayerBody.model");

	/*움직임*/
	Animator->Add_AnimClips("GamePlay_Level", "Base_Wait.anim", "Player", true);

	Animator->Add_AnimClips("GamePlay_Level", "Move_Run_F.anim", "Player", true);
	Animator->Add_AnimClips("GamePlay_Level", "Move_Dash_F.anim", "Player", true);
	Animator->Add_AnimClips("GamePlay_Level", "MoveTurn_Run_L.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "MoveTurn_Dash_L.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToStop_RunLatter_L.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToStop_DashLatter_L.anim", "Player", false);

	/*툴 = Axe*/
	Animator->Add_AnimClips("GamePlay_Level", "ToolAxe_Air.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolAxe_APose.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolAxe_Hit.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolAxe_Ready.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolAxe_ReadyKeep.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolAxe_Repelled.anim", "Player", false);

	/*툴 = NET*/
	Animator->Add_AnimClips("GamePlay_Level", "ToolNet_APose.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolNet_AimWalk_F.anim", "Player", true);
	Animator->Add_AnimClips("GamePlay_Level", "ToolNet_Get.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolNet_Slip.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolNet_Swing.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolNet_SwingStop_Ground.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolNet_SwingStop_Upper.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolNet_SwingStop_Lower.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolNet_SwingStop_Middle.anim", "Player", false);

	/*툴 = SCOOP*/
	Animator->Add_AnimClips("GamePlay_Level", "ToolScoop_APose.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolScoop_Air.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolScoop_BuryHole.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolScoop_Dig.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolScoop_DigStump.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "ToolScoop_Repelled.anim", "Player", false);

	Animator->Add_AnimClips("GamePlay_Level", "Tree_Shake.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Tree_ShakeReadyKeep.anim", "Player", true);
	Animator->Add_AnimClips("GamePlay_Level", "Base_EquipOff.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Base_EquipOn.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Menu_Think.anim", "Player", true);
	Animator->Add_AnimClips("GamePlay_Level", "Pickup.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Pickup_Get.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Generic_Get.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Generic_GetKeep.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Generic_PullOut.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Generic_Putaway.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Generic_PutawayKeep.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Transfer_Eat.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Menu_Eat.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Transfer_ReceiveForward.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Transfer_ReceiveReturn.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Transfer_ReceiveBack.anim", "Player", false);
	Animator->Add_AnimClips("GamePlay_Level", "Transfer_Putaway.anim", "Player", false);

	Animator->Add_AnimClips("GamePlay_Level", "Generic_Putaway.anim", "Player", true);
	Animator->Add_AnimClips("GamePlay_Level", "Etc_DiyCreating.anim", "Player", true);
	Animator->Add_AnimClips("GamePlay_Level", "Etc_DiyFinish.anim", "Player", false);
}

void CPlayer::Add_MaterialAnim()
{
	/*머티리얼 애니메이션 클림좀 다르게 해야할 듯. 키프레임 만 지정 / 인덱스 지정/ 방식응로*/
	MATERIAL_CLIP clip = {};
	clip.AnimationKeyFrame = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,2,2 };
	clip.fDuration = 50.f;
	clip.TickperSecond = 15.f;
	clip.isLoop = true;
	Get_Component<CMaterialAnimator>()->RegisterKeyFrame("mEye", "IDLE", clip);
	Get_Component<CMaterialAnimator>()->Change_Animation("mEye", "IDLE");
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

	PLAYER_PARTS_DESC* pGlassDesc = new PLAYER_PARTS_DESC;
	pGlassDesc->pPlayer = this;

	CClothParts::CLOTHES_DESC* pTopDesc = new CClothParts::CLOTHES_DESC;
	pTopDesc->pPlayer = this;
	pTopDesc->ClothType = "PlayerTopsTopYshirtsL";

	CClothParts::CLOTHES_DESC* pBottomDesc = new CClothParts::CLOTHES_DESC;
	pBottomDesc->pPlayer = this;
	pBottomDesc->ClothType = "PlayerBottomsPantsNormal";

	m_InfoPack.pRightHand = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_PlayerPart_Hand" })
		.Add_ObjDesc(pRHandDesc)
		.Build("Right_Hand");
	m_InfoPack.pLeftHand = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_PlayerPart_Hand" })
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

	Adjust_Cloth_Material(pTop, "Work_mTops", "mTops");
	CGameObject* pBottom = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_ClothParts" })
		.Add_ObjDesc(pBottomDesc)
		.Build("Bottom");
	Adjust_Cloth_Material(pBottom, "Sweat_mBottoms", "mBottoms");

	CGameObject* pGlass = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_GlassParts" })
		.Add_ObjDesc(pGlassDesc)
		.Build("Glass");

	Get_Component<CObjectContainer>()->Add_Child(m_InfoPack.pRightHand, false);
	Get_Component<CObjectContainer>()->Add_Child(m_InfoPack.pLeftHand, false);
	Get_Component<CObjectContainer>()->Add_Child(pHair, false);
	Get_Component<CObjectContainer>()->Add_Child(pHairCap, false);
	Get_Component<CObjectContainer>()->Add_Child(pTop, true);
	Get_Component<CObjectContainer>()->Add_Child(pBottom, true);
	Get_Component<CObjectContainer>()->Add_Child(pGlass, false);
}

void CPlayer::Add_Inventory()
{
	CUI_Object* pUI = Builder::Create_UIObject({ "GamePlay_Level","GamePlay_GameUI_PlayerInventory" })
		.Add_To_Level("GamePlay_Level")
		.Build("Inventory");

	CGameInstance::GetInstance()->Get_UIMgr()->Add_UIObject(pUI, "GamePlay_Level");
	m_pInventory = dynamic_cast<CPlayer_Inventory*>(pUI);
	m_pInventory->Set_Player(this);
}

void CPlayer::Set_TargetCamera()
{
	CTarget_Camera::TARGET_CAM_DESC* pCamDesc = new CTarget_Camera::TARGET_CAM_DESC;
	pCamDesc->pTarget = this;
	pCamDesc->vOffset = { 0,50,50,0 };

	CGameObject* pCamera = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_TargetCamera" })
		.Camera({ (float)Client::g_iWinSizeX / Client::g_iWinSizeY })
		.Add_ObjDesc(pCamDesc)
		.Build("Target_Cam");

	m_pCamera = dynamic_cast<CTarget_Camera*>(pCamera);
	Safe_AddRef(m_pCamera);
	Get_Component<CObjectContainer>()->Add_Child(pCamera, false);
	CGameInstance::GetInstance()->Get_CameraMgr()->Set_MainCam(pCamera->Get_Component<CCamera>());

	CTarget_Camera::TARGET_CAM_DESC* psCamDesc = new CTarget_Camera::TARGET_CAM_DESC;
	psCamDesc->pTarget = this;
	psCamDesc->vOffset = { 0,150,150,0 };

	/*햇빛으로 사용해야하는데 나중에 고치자.*/
	CGameObject* psunCamera = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_TargetCamera" })
		.Camera({ (float)Client::g_iWinSizeX / Client::g_iWinSizeY })
		.Add_ObjDesc(psCamDesc)
		.Build("Target_Cam");
	Get_Component<CObjectContainer>()->Add_Child(psunCamera, false);

	CGameInstance::GetInstance()->Get_CameraMgr()->Set_ShadowCam(psunCamera->Get_Component<CCamera>());
}

void CPlayer::Adjust_Cloth_Material(CGameObject* pObject, string TextureKey, string subsetKey)
{
	auto instance = pObject->Get_Component<CMaterial>()->Find_MaterialByName(subsetKey);
	if (!instance) return;

	CTexture* pDiffuse = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture("GamePlay_Level", TextureKey + "_Alb.dds");
	CTexture* pMixture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture("GamePlay_Level", TextureKey + "_Mix.dds");
	CTexture* pNormal = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture("GamePlay_Level", TextureKey + "_Nrm.dds");
	CTexture* pOpcity = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture("GamePlay_Level", TextureKey + "_OP.dds");

	SHADER_PARAM param = {};
	param.iSize = 0;
	param.typeName = "Texture2D";

	param.pData = pDiffuse->Get_SRV();
	instance->Set_Param("DiffuseTexture", param);

	param.pData = pMixture->Get_SRV();
	instance->Set_Param("MixtureTexture", param);

	param.pData = pNormal->Get_SRV();
	instance->Set_Param("NormalTexture", param);

	param.pData = pOpcity->Get_SRV();
	instance->Set_Param("OpacityTexture", param);
}

void CPlayer::OnEventAction(const BaseEvent& event)
{
	if (event.eType == EVENT_TYPE::DialougueEnd) {
		const auto& evt = static_cast<const OnEndDialogue&>(event);
		if (m_InfoPack.isTalking && evt.pSpeaker == m_InfoPack.pTalker)
		{
			m_InfoPack.isTalking = false; m_InfoPack.pTalker = nullptr;
		}
	}

	if (event.eType == EVENT_TYPE::NoticeDialogue) {
		const auto& evt = static_cast<const OnNoticeDialogue&>(event);
		if (evt.pSubject == this && !m_InfoPack.isTalking) {
			m_InfoPack.pTalker = evt.pCounter;
			m_pStateMachine->Request_ChangeState(STATE_LAYER::ACTION, "Interact_Talking_State");
		}
	}

	if (event.eType == EVENT_TYPE::TransItem) {
		const auto& evt = static_cast<const TRANS_ITEM&>(event);
		if (evt.pObject) {
			m_InfoPack.m_nowTrans = evt;
			m_pStateMachine->Request_ChangeState(STATE_LAYER::ACTION, "Action_TransGet_State");
		}
	}
}

void CPlayer::Open_Craft()
{
	m_ControlPack.MsgForceBlock = true;
	m_InfoPack.isCrafting = true;

	auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
	if (!nowLevel)
		return;

	CRAFT_DATA_DESC craftDesc = {};
	craftDesc.InvenData = m_pInventory->Get_All_InventoryData();
	craftDesc.OnClose = [this](const CRAFT_RESULT& result) {Close_Craft(result); };

	nowLevel->Get_LevelObject<CUI_Responcer>()->Active_UI("Craft_UI", &craftDesc);
}

void CPlayer::Close_Craft(const CRAFT_RESULT& result)
{
	m_ControlPack.MsgForceBlock = false;
	m_InfoPack.isCrafting = false;

	if (result.Result_ItemTag.empty()) {
		m_pStateMachine->Request_ChangeState(STATE_LAYER::ACTION, "Movement_Idle_State");
		return;
	}

	m_pInventory->PullOut_Item(result.ResourceItem_01, result.ResourceItemCount_01);
	m_pInventory->PullOut_Item(result.ResourceItem_02, result.ResourceItemCount_02);
	m_pInventory->PullOut_Item(result.ResourceItem_03, result.ResourceItemCount_03);

	auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();

	m_InfoPack.pObjectOnLeftHand = nowLevel->Get_LevelObject<CItemSpawner>()->SpawnItem(result.Result_ItemTag);
	m_InfoPack.pObjectOnLeftHand->Get_Component<CModel>()->Set_CompActive(false);
	m_pStateMachine->Request_ChangeState(STATE_LAYER::ACTION, "Action_Craft_State");
}

void CPlayer::BroadCast_Position()
{
	auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
	if (!nowLevel)
		return;
	auto EventSys = nowLevel->Get_LevelObject<CEventSystem>();
	PLAYER_POS event{ EVENT_TYPE::Player_Pos,Get_Position(),this };

	EventSys->OnBroadCast<BaseEvent>(event);
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
	Safe_Release(m_pCamera);
}