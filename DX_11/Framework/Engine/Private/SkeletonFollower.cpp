#include "SkeletonFollower.h"
#include "SkeletalModel.h"
#include "ModelData.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "IResourceService.h"

CSkeletonFollower::CSkeletonFollower()
{
}

CSkeletonFollower::CSkeletonFollower(const CSkeletonFollower& rhs)
{
}

CSkeletonFollower::~CSkeletonFollower()
{
}

HRESULT CSkeletonFollower::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkeletonFollower::Initialize(COMPONENT_DESC* pArg)
{
	return S_OK;
}

HRESULT CSkeletonFollower::Link_MyModel(const string& levelKey, const string& modelDataKey)
{
	Safe_Release(m_pMyData);
	m_pMyData = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_ModelData(levelKey, modelDataKey);
	Safe_AddRef(m_pMyData);
	m_ReflectedIndices.resize(m_pMyData->Get_BoneCount());
	_float4x4 baseMatrix = {};
	XMStoreFloat4x4(&baseMatrix, XMMatrixIdentity());
	m_ReflectedMatices.resize(m_pMyData->Get_BoneCount(), baseMatrix);
	return S_OK;
}

HRESULT CSkeletonFollower::Link_MasterModel(const string& levelKey, const string& modelDataKey)
{
	Safe_Release(m_pMasterData);
	m_pMasterData = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_ModelData(levelKey, modelDataKey);
	Safe_AddRef(m_pMasterData);
	m_ReflectedIndices= m_pMyData->GenerateFollowingIndices(m_pMasterData);

	return S_OK;
}

HRESULT CSkeletonFollower::Link_MyModel(CSkeletalModel* pModel)
{
	Safe_Release(m_pMyData);
	m_pMyData = pModel->Get_ModelData();
	Safe_AddRef(m_pMyData);
	m_ReflectedIndices.resize(m_pMyData->Get_BoneCount(),0);
	_float4x4 baseMatrix = {};
	XMStoreFloat4x4(&baseMatrix, XMMatrixIdentity());
	m_ReflectedMatices.resize(m_pMyData->Get_BoneCount(), baseMatrix);
	return S_OK;
}

HRESULT CSkeletonFollower::Link_MasterModel(CSkeletalModel* pMasterModel)
{
	Safe_Release(m_pMasterData);
	m_pMasterData = pMasterModel->Get_ModelData();
	Safe_AddRef(m_pMasterData);
	m_ReflectedIndices = m_pMyData->GenerateFollowingIndices(m_pMasterData);

	return S_OK;
}


void CSkeletonFollower::Set_MasterAnimator(CAnimator3D* pAnimator)
{
		Safe_Release(m_pMasterAnimator);
		m_pMasterAnimator = pAnimator;
		Safe_AddRef(m_pMasterAnimator);
}

void CSkeletonFollower::Sync_Bones(_float dt)
{
	auto& masterVec = m_pMasterAnimator->Get_BoneMatrices();

	for (size_t i = 0; i < m_ReflectedIndices.size(); i++)
	{
		if (m_ReflectedIndices[i] == -1) { continue; }
		m_ReflectedMatices[i] = masterVec[m_ReflectedIndices[i]];
	}
}

CSkeletonFollower* CSkeletonFollower::Create()
{
	CSkeletonFollower* instance = new CSkeletonFollower();
	if (FAILED(instance->Initialize_Prototype())) {
		Safe_Release(instance);
	}
	return instance;
}

CComponent* CSkeletonFollower::Clone()
{
	return new CSkeletonFollower(*this);
}

void CSkeletonFollower::Free()
{
	Safe_Release(m_pMyData);
	Safe_Release(m_pMasterData);
	Safe_Release(m_pMasterAnimator);
}
