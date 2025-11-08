#include "Client_Defines.h"
#include "ClothParts.h"

#include "SkeletalModel.h"
#include "SkeletonFollower.h"
#include "Material.h"
#include "Animator3D.h"

#include "Player.h"
CClothParts::CClothParts()
{
}

CClothParts::CClothParts(const CClothParts& rhs)
	:CGameObject(rhs)
{
}

CClothParts::~CClothParts()
{
}

HRESULT CClothParts::Initialize_Prototype()
{
	Add_Component<CSkeletalModel>();
	Add_Component<CMaterial>();

	return S_OK;
}

HRESULT CClothParts::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Add_Component<CSkeletonFollower>();
	CLOTHES_DESC* pDesc = static_cast<CLOTHES_DESC*>(pArg);

	Get_Component<CSkeletalModel>()->Link_Model("GamePlay_Level", pDesc->ClothType + ".model");
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", pDesc->ClothType + ".mat");
	Get_Component<CSkeletalModel>()->ShadowCast(true);

	Get_Component<CSkeletonFollower>()->Link_MyModel(Get_Component<CSkeletalModel>());
	Get_Component<CSkeletonFollower>()->Link_MasterModel(pDesc->pPlayer->Get_Component<CSkeletalModel>());
	Get_Component<CSkeletonFollower>()->Set_MasterAnimator(pDesc->pPlayer->Get_Component<CAnimator3D>());
	return S_OK;
}

void CClothParts::Priority_Update(_float dt)
{

}

void CClothParts::Update(_float dt)
{
	CSkeletonFollower* Follower = Get_Component<CSkeletonFollower>();
	if (Follower)
		Follower->Sync_Bones(dt);

	m_pTransform->Get_WorldMatrix_Ptr();
}

void CClothParts::Late_Update(_float dt)
{
}

void CClothParts::Render_GUI()
{
	__super::Render_GUI();
}

CClothParts* CClothParts::Create()
{
	CClothParts* instance = new CClothParts();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CHairParts");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CClothParts::Clone(INIT_DESC* pArg)
{
	CClothParts* instance = new CClothParts(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CHairParts");
		Safe_Release(instance);
	}

	return instance;
}

void CClothParts::Free()
{
	__super::Free();
}
