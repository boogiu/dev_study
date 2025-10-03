#include "Loader_Defines.h"
#include "PartsObject.h"
#include "LoadSkeletalModel.h"
#include "SkeletonFollower.h"

#include "ModelData.h"
#include"Animator3D.h" 

CPartsObject::CPartsObject()
{
}

CPartsObject::CPartsObject(const CPartsObject& rhs)
	:CModelObject(rhs)
{
}

HRESULT CPartsObject::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CPartsObject::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	Add_Component<CSkeletonFollower>();

	return S_OK;
}

void CPartsObject::Priority_Update(_float dt)
{
	__super::Priority_Update(dt);

}

void CPartsObject::Update(_float dt)
{
	__super::Update(dt);
	CSkeletonFollower* Follower = Get_Component<CSkeletonFollower>();
	if(Follower)
		Follower->Sync_Bones(dt);
}

void CPartsObject::Late_Update(_float dt)
{
	__super::Late_Update(dt);
}

HRESULT CPartsObject::Load_Animated(const string& fileName)
{
	ReleasPrevModel();
	CLoadSkeletalModel* pModel = Add_Component< CLoadSkeletalModel>();
	pModel->Load_Model(m_pAIScene, fileName);
	CSkeletonFollower* Follower = Get_Component<CSkeletonFollower>();
	Follower->Link_MyModel(pModel);
	Follower->Link_MasterModel(m_pMasterModel);
	Follower->Set_MasterAnimator(m_pMasterAnimator);
	return S_OK;
}

void CPartsObject::Inject_Master(CSkeletalModel* pModel, CAnimator3D* pAnimator)
{
	Safe_Release(m_pMasterModel);
	Safe_Release(m_pMasterAnimator);

	m_pMasterModel = pModel;
	m_pMasterAnimator = pAnimator;

	Safe_AddRef(m_pMasterModel);
	Safe_AddRef(m_pMasterAnimator);
}

void CPartsObject::Render_GUI()
{
	__super::Render_GUI();
}

CPartsObject* CPartsObject::Create()
{
	CPartsObject* instance = new CPartsObject();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CPartsObject");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CPartsObject::Clone(INIT_DESC* pArg)
{
	CPartsObject* instance = new CPartsObject(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CPartsObject");
		Safe_Release(instance);
	}

	return instance;
}
void CPartsObject::Free()
{
	__super::Free();
	Safe_Release(m_pMasterModel);
	Safe_Release(m_pMasterAnimator);
}
