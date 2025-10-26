#include"Client_Defines.h"
#include "PlayerPart_Hand.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "BoneFollower.h"
#include "Animator3D.h"
#include "ObjectContainer.h"

#include "Player.h"
#include "ToolItem.h"
#include "Child.h"
#include "Collider.h"
CPlayerPart_Hand::CPlayerPart_Hand()
{
}

CPlayerPart_Hand::CPlayerPart_Hand(const CPlayerPart_Hand& rhs)
:CGameObject(rhs)
{
}

CPlayerPart_Hand::~CPlayerPart_Hand()
{
}

HRESULT CPlayerPart_Hand::Initialize_Prototype()
{
	Add_Component<CBoneFollower>();
	Add_Component<CObjectContainer>();
	return S_OK;
}

HRESULT CPlayerPart_Hand::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	CPlayer::PLAYER_PARTS_DESC* pDesc = static_cast<CPlayer::PLAYER_PARTS_DESC*>(pArg);
	Get_Component<CBoneFollower>()->Link_Bone(
		pDesc->pPlayer->Get_Component<CAnimator3D>(),
		"Armature_Hand_R"
	);

	CGameObject* pTool = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_PlayerTool" })
		.Rotate({ 0 ,XMConvertToRadians(180),0})
		.Build("Tool");

	m_pToolItem = dynamic_cast<CToolItem*>(pTool);
	Get_Component<CObjectContainer>()->Add_Child(pTool, true);

	return S_OK;
}

void CPlayerPart_Hand::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CPlayerPart_Hand::Update(_float dt)
{
	Get_Component<CBoneFollower>()->Sync_Transform(dt, m_pTransform);
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CPlayerPart_Hand::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CPlayerPart_Hand::Render_GUI()
{
	
}

void CPlayerPart_Hand::Change_Item(ITEM_DATA_DESC data)
{
	m_pToolItem->Set_Item(data);
	m_eItemType = data.eType;
}

void CPlayerPart_Hand::Active_ColliderTool(_bool Active)
{
	m_pToolItem->Get_Component<CCollider>()->Set_ColliderActive(Active);
}


CPlayerPart_Hand* CPlayerPart_Hand::Create()
{
	CPlayerPart_Hand* instance = new CPlayerPart_Hand();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CPlayerPart_Hand");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CPlayerPart_Hand::Clone(INIT_DESC* pArg)
{
	CPlayerPart_Hand* instance = new CPlayerPart_Hand(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CPlayerPart_Hand");
		Safe_Release(instance);
	}

	return instance;
}

void CPlayerPart_Hand::Free()
{
	__super::Free();
}
