#include "LoadedSkeleton.h"
#include "LoadBone.h"
CLoadedSkeleton::CLoadedSkeleton()
{
}

HRESULT CLoadedSkeleton::Ready_Bones(const aiNode* pAINode, _int iParentIndex)
{
    CLoadBone* pBone = CLoadBone::Create(pAINode, iParentIndex);
    if (nullptr == pBone)
        return E_FAIL;

    m_Bones.push_back(pBone);

    _int iPIndex = m_Bones.size() - 1;

    for (_uint i = 0; i < pAINode->mNumChildren; ++i)
    {
        Ready_Bones(pAINode->mChildren[i], iPIndex);
    }

    m_BoneMap.emplace(pAINode->mName.data, iPIndex);
    return S_OK;
}

HRESULT CLoadedSkeleton::Create_Bone(const string& MeshName)
{
    return E_NOTIMPL;
}

CLoadedSkeleton* CLoadedSkeleton::Create(const aiNode* pAINode)
{
	CLoadedSkeleton* instance = new CLoadedSkeleton;
	if (FAILED(instance->Ready_Bones(pAINode, -1))) {
		Safe_Release(instance);
	}
	return instance;
}

void CLoadedSkeleton::Free()
{
	__super::Free();
}
