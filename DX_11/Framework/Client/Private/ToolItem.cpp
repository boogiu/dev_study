#include "Client_Defines.h"
#include "ToolItem.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "BoneFollower.h"
#include "Animator3D.h"

#include "Player.h"
#include "OBB_Collider.h"
#include "ObjectContainer.h"
#include "FishSub_Tool.h"

CToolItem::CToolItem()
{
}

CToolItem::CToolItem(const CToolItem& rhs)
	:CGameObject(rhs)
{
}

CToolItem::~CToolItem()
{
}

HRESULT CToolItem::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CSkeletalModel>();
	Add_Component<CMaterial>();
	Add_Component<COBB_Collider>();
	Add_Component<CObjectContainer>();
	Add_Component<CAnimator3D>();

	return S_OK;
}

HRESULT CToolItem::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	
	m_InstanceTag = "None";
	m_pOwner = static_cast<CHRACTER_TOOL_DESC*>(pArg)->pOwner;
	Get_Component<COBB_Collider>()->Make_MinMaxCollider({ {-2,-2,-2},{2,2,2} });
	Get_Component<CSkeletalModel>()->ShadowCast(true);
	Get_Component<COBB_Collider>()->Set_ColliderActive(false);

	Get_Component<CAnimator3D>()->Set_CompActive(false);

	CGameObject* pSub =
		Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_FishSub" })
		.Position({ 550, 10, 550 })
		.Build("Sub",&m_SubIndex);

	Get_Component<CObjectContainer>()->Add_Child(pSub, false);
	pSub->Get_Component<CModel>()->Set_CompActive(false);

	m_pSubTool = dynamic_cast<CFishSub_Tool*>(pSub);
	return S_OK;
}

void CToolItem::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CToolItem::Update(_float dt)
{
	if(Get_Component<CAnimator3D>()->Get_CompActive())
	Get_Component<CAnimator3D>()->Update_Animation(dt);

	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CToolItem::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CToolItem::Render_GUI()
{
	Get_Component<COBB_Collider>()->Render_GUI();
	Get_Component<CMaterial>()->Render_GUI();
	Get_Component<CModel>()->Render_GUI();

	if (Get_Component<CAnimator3D>()->Get_CompActive())
		Get_Component<CAnimator3D>()->Render_GUI();
}

void CToolItem::AdjustByItem(itemType type)
{
	//m_pTransform->Reset_Rotation();
	m_pTransform->Set_Pos({0,0,0});

	if (type == itemType::FishingRod) {
		m_pTransform->Override_Rotation({ 0,1,0,0 }, XMConvertToRadians(180));
	}else{
		m_pTransform->Override_Rotation({ 0,1,0,0 }, XMConvertToRadians(0));
	}
}

void CToolItem::Set_Item(TOOL_DATA_DESC data)
{
	switch (data.TypeTag)
	{
	case itemType::None:
		m_InstanceTag = "None";
		break;
	case itemType::Fruit:
		m_InstanceTag = "None";
		break;
	case itemType::Axe:
		m_InstanceTag = "Axe";
		break;
	case itemType::Scoop:
		m_InstanceTag = "Scoop";
		break;
	case itemType::Net:
		m_InstanceTag = "Net";
		break;
	case itemType::FishingRod:
		m_InstanceTag = "FishingRod";
		break;
	default:
		break;
	}

	Get_Component<COBB_Collider>()->Set_ColliderActive(false);

	if (data.TypeTag == itemType::None) {
		Get_Component<CModel>()->Set_CompActive(false);
		return;
	}
	else {
		AdjustByItem(data.TypeTag);
		Get_Component<CModel>()->Set_CompActive(true);
	}

	Get_Component<CModel>()->Link_Model("GamePlay_Level", data.modelName);
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", data.materialName);
	Get_Component<CCollider>()->Make_MinMaxCollider(Get_Component<CModel>()->Get_LocalBoundingBox());

	if (data.TypeTag == itemType::FishingRod) {
	
		Get_Component<CAnimator3D>()->LinkAnimate_Model("GamePlay_Level", data.modelName);
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolPoleAnim_APose.anim", "FishingRod");
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolPoleAnim_Aim.anim", "FishingRod",true);
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolPoleAnim_Air.anim", "FishingRod");
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolPoleAnim_Catch.anim", "FishingRod");
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolPoleAnim_CatchKeep.anim", "FishingRod", true);
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolPoleAnim_CatchKeepBig.anim", "FishingRod", true);
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolPoleAnim_Complete.anim", "FishingRod");
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolPoleAnim_CompleteKeep.anim", "FishingRod", true);
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolPoleAnim_Get.anim", "FishingRod");
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolPoleAnim_GetKeep.anim", "FishingRod", true);
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolPoleAnim_Hit.anim", "FishingRod", true);
		Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "ToolPoleAnim_Swing.anim", "FishingRod");

		Get_Component<CAnimator3D>()->Set_CompActive(true);
		Get_Component<CAnimator3D>()->Change_Animation("ToolPoleAnim_APose.anim");

		m_pSubTool->Get_Component<CModel>()->Set_CompActive(true);
		m_pSubTool->Sync_Bont_To_Rod(Get_Component<CAnimator3D>(), "Armature_Sub");
	}
	else {
		Get_Component<CAnimator3D>()->Set_CompActive(false);
	}
}

void CToolItem::Change_Tool_Animation(const string& animName)
{
	Get_Component<CAnimator3D>()->Change_Animation(animName);
}

void CToolItem::OnCollisionEnter(COLLISION_CONTEXT context)
{
	if (m_pOwner) {
		context.EventTag += "_Tool";
		m_pOwner->OnCollisionEnter(context);
	}
}

void CToolItem::OnCollisionStay(COLLISION_CONTEXT context)
{
	if (m_pOwner) {
		context.EventTag += "_Tool";
		m_pOwner->OnCollisionStay(context);
	}
}

void CToolItem::OnCollisionExit(COLLISION_CONTEXT context)
{
	if (m_pOwner) {
		context.EventTag += "_Tool";
		m_pOwner->OnCollisionExit(context);
	}
}

CToolItem* CToolItem::Create()
{
	CToolItem* instance = new CToolItem();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CToolItem");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CToolItem::Clone(INIT_DESC* pArg)
{
	CToolItem* instance = new CToolItem(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CToolItem");
		Safe_Release(instance);
	}

	return instance;
}

void CToolItem::Free()
{
	__super::Free();
}