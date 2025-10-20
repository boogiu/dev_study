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

	return hr;
}

HRESULT CPlayer::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);


	m_pStateMachine = CPlayerStateMachine::Create(this);
	Add_AnimationClips();
	Add_PartObjects();

	Get_Component<CSkeletalModel>()->SetDrawable(5,false);
	Get_Component<CSkeletalModel>()->SetDrawable(8,false);
	Get_Component<CSkeletalModel>()->SetDrawable(10,false);


	CMaterialInstance* SkinInstance  =Get_Component<CMaterial>()->Find_MaterialByName("mSkin");
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
	return S_OK;
}

void CPlayer::Priority_Update(_float dt)
{

	auto pInput = CGameInstance::GetInstance()->Get_InputDev();

	_float2 moveAxis = { 0.f, 0.f };
	if (pInput->Key_Down(VK_SPACE)) {
		Get_Component<CAnimator3D>()->Chane_Animation("ToolAxe_Air.anim");
	}
	if (pInput->Key_Down(VK_UP))				moveAxis.y -= 1.f;
	if (pInput->Key_Down(VK_DOWN))		moveAxis.y += 1.f;
	if (pInput->Key_Down(VK_LEFT))			moveAxis.x += 1.f;
	if (pInput->Key_Down(VK_RIGHT))		moveAxis.x -= 1.f;

	m_pStateMachine->SetInput(moveAxis);

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

	m_pStateMachine->Render_StateGUI(this);
}

void CPlayer::Add_AnimationClips()
{
	Get_Component<CAnimator3D>()->LinkAnimate_Model("GamePlay_Level", "PlayerBody.model");
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Act_WatchCStd.anim", "Player", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Move_Run_F.anim", "Player", true);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "MoveTurn_Run_L.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToStop_RunFirst_L.anim", "Player", false);
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolAxe_Air.anim", "Player", false);
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

	CGameObject* pTool = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_PlayerTool" })
		.Add_ObjDesc(pToolDesc)
		.Build("Tool");

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
	Get_Component<CObjectContainer>()->Add_Child(pTop,true);
	Get_Component<CObjectContainer>()->Add_Child(pBottom,true);

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