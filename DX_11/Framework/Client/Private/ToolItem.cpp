#include "Client_Defines.h"
#include "ToolItem.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "BoneFollower.h"
#include "Animator3D.h"

#include "Player.h"
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
	HRESULT hr = Add_Component<CSkeletalModel>()->Link_Model("GamePlay_Level", "ToolAxeFirst.model");
	hr = Add_Component<CMaterial>()->Link_Material("GamePlay_Level", "ToolAxeFirst.mat");
	Add_Component<CBoneFollower>();

	return hr;
}

HRESULT CToolItem::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	CPlayer::PLAYER_PARTS_DESC* pDesc = static_cast<CPlayer::PLAYER_PARTS_DESC*>(pArg);
	Get_Component<CBoneFollower>()->Link_Bone(
		pDesc->pPlayer->Get_Component<CAnimator3D>(),
		"Armature_Hand_L"
	);

	Get_Component<CBoneFollower>()->Set_Offset(
		XMMatrixRotationX(XMConvertToRadians(180))
	);
	return S_OK;
}

void CToolItem::Priority_Update(_float dt)
{
}

void CToolItem::Update(_float dt)
{
	Get_Component<CBoneFollower>()->Sync_Transform(dt, m_pTransform);
}

void CToolItem::Late_Update(_float dt)
{
}

void CToolItem::Render_GUI()
{
	__super::Render_GUI();
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