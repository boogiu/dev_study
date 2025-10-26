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
#include "OBB_Collider.h"

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
	return hr;
}

HRESULT CPlayer::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);


	m_pStateMachine = CPlayerStateMachine::Create(this);
	Add_AnimationClips();
	Add_PartObjects();

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

	Get_Component<COBB_Collider>()->Make_MinMaxCollider(
		{ { -5,0,0 }, {5,5,6} }
	);
	return S_OK;
}

void CPlayer::Priority_Update(_float dt)
{

	auto pInput = CGameInstance::GetInstance()->Get_InputDev();

	_float2 moveAxis = { 0.f, 0.f };

	if (pInput->Key_Down(VK_UP))				moveAxis.y = -1.f;
	if (pInput->Key_Down(VK_DOWN))		moveAxis.y = +1.f;
	if (pInput->Key_Down(VK_LEFT))			moveAxis.x = +1.f;
	if (pInput->Key_Down(VK_RIGHT))		moveAxis.x = -1.f;

	XMStoreFloat2(&m_vInputAxis, XMVector2Normalize(XMLoadFloat2(&moveAxis)));

	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CPlayer::Update(_float dt)
{
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
		Data.TypeTag = "";
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

	ImGui::End();
}

ITEM_TYPE CPlayer::Get_CurrentItemType()
{
	CGameObject* pHand = Get_Component<CObjectContainer>()->Find_ObjectByName("Right_Hand");
	CPlayerPart_Hand* pHandPart = dynamic_cast<CPlayerPart_Hand*>(pHand);

	if (pHandPart)
		return pHandPart->Get_CurrentItemType();

	return ITEM_TYPE::NONE;
}

void CPlayer::Change_Item(ITEM_DATA_DESC desc)
{
	if (m_CurItem.modelName == desc.modelName) {
		return;
	}
	m_DstItem = desc;
	m_pStateMachine->Request_ChangeState("Transfer_Item_State");
}

void CPlayer::Set_CurItemData(ITEM_DATA_DESC desc)
{
	if (m_CurItem.modelName == desc.modelName) {
		return;
	}

	m_CurItem = desc;
	m_DstItem = {};
	CGameObject* pObj = Get_Component<CObjectContainer>()->Find_ObjectByName("Right_Hand");
	dynamic_cast<CPlayerPart_Hand*>(pObj)->Change_Item(desc);
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

void CPlayer::ActiveCollider_Tool(_bool active)
{
	CGameObject* pHand = Get_Component<CObjectContainer>()->Find_ObjectByName("Right_Hand");
	CPlayerPart_Hand* pHandPart = dynamic_cast<CPlayerPart_Hand*>(pHand);
	pHandPart->Active_ColliderTool(active);
}

void CPlayer::OnCollisionEnter(CGameObject* pObject)
{
	if (pObject->Has_Tag("Tree")) {
		int i = 0;
	}
}

void CPlayer::OnCollisionStay(CGameObject* pObject)
{
	if (pObject->Has_Tag("Tree")) {
		int i = 0;
	}
}

void CPlayer::OnCollisionExit(CGameObject* pObject)
{
	if (pObject->Has_Tag("Tree")) {
		int i = 0;
	}
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

	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Base_EquipOn.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Base_EquipOff.anim", "Player", false);
}

void CPlayer::Add_PartObjects()
{
	PLAYER_PARTS_DESC* pToolDesc = new PLAYER_PARTS_DESC;
	pToolDesc->pPlayer = this;

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

	CGameObject* pTool = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_PlayerPart_Hand" })
		.Add_ObjDesc(pToolDesc)
		.Build("Right_Hand");

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

	Get_Component<CObjectContainer>()->Add_Child(pTool, false);
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