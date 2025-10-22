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
	Add_Component<CSkeletalModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CToolItem::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	/*CPlayer::PLAYER_PARTS_DESC* pDesc = static_cast<CPlayer::PLAYER_PARTS_DESC*>(pArg);
	Get_Component<CBoneFollower>()->Link_Bone(
		pDesc->pPlayer->Get_Component<CAnimator3D>(),
		"Armature_Hand_L"
	);

	Get_Component<CBoneFollower>()->Set_Offset(
		XMMatrixRotationX(XMConvertToRadians(180))
	);*/

	return S_OK;
}

void CToolItem::Priority_Update(_float dt)
{
}

void CToolItem::Update(_float dt)
{
}

void CToolItem::Late_Update(_float dt)
{
}

void CToolItem::Render_GUI()
{
	//__super::Render_GUI();
}

void CToolItem::Set_Item(ITEM_DATA_DESC data)
{
	if (data.eType == ITEM_TYPE::NONE) {
		Get_Component<CModel>()->Set_Active(false);
		return;
	}
	else {
		Get_Component<CModel>()->Set_Active(true);
	}

	Get_Component<CModel>()->Link_Model("GamePlay_Level", data.modelName);
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", data.materialName);
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