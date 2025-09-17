#include "AnimatedModel.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "IRenderService.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Bone.h"

CAnimatedModel::CAnimatedModel()
{
}

CAnimatedModel::CAnimatedModel(const CAnimatedModel& rhs)
	: CModel(rhs), m_pSkeleton{ rhs.m_pSkeleton }, m_FinalBoneMatrices{ rhs.m_FinalBoneMatrices }
{
}

HRESULT CAnimatedModel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimatedModel::Initialize(COMPONENT_DESC* pArg)
{
	return S_OK;
}

HRESULT CAnimatedModel::Link_Buffer(const string& levelKey, const string& MeshKey)
{
	m_Buffers = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Mesh(levelKey, MeshKey);
	for (CMesh* mesh : m_Buffers) {
		m_DrawableMeshes.push_back(true);
		Safe_AddRef(mesh);
	}

	m_pSkeleton = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Skeleton(levelKey, MeshKey);
	Safe_AddRef(m_pSkeleton);
	m_FinalBoneMatrices.resize(m_Buffers.size());
	return S_OK;
}

HRESULT CAnimatedModel::Render_Mesh(ID3D11DeviceContext* pContext, _uint Index)
{
	if (Index >= m_Buffers.size()) return E_FAIL;
	m_Buffers[Index]->Bind_Buffer(pContext);
	m_Buffers[Index]->Render(pContext);
	return S_OK;
}

void CAnimatedModel::Update_Animation(_float dt)
{
	//skeletonUpdate
	m_pSkeleton->Update_CombinedMatrix(dt);
}


const vector<_float4x4>& CAnimatedModel::Get_BoneMatrices(_uint DrawIndex)
{
	return m_Buffers[DrawIndex]->Bind_BoneMatrices(m_pSkeleton->Get_Bones());
}


CAnimatedModel* CAnimatedModel::Create()
{
	CAnimatedModel* instance = new CAnimatedModel();

	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("CStaticModel Create Failed : CStaticModel");
		Safe_Release(instance);
	}

	return instance;
}

CComponent* CAnimatedModel::Clone()
{
	CAnimatedModel* instance = new CAnimatedModel(*this);
	return instance;
}

void CAnimatedModel::Free()
{
	__super::Free();
	Safe_Release(m_pSkeleton);
}
