#include "LoadBone.h"

CLoadBone::CLoadBone()
{
}

HRESULT CLoadBone::Initialize(const aiNode* pAINode, _int iParentIndex)
{
	m_iParentBoneIndex = iParentIndex;
	m_BoneName = pAINode->mName.data;

	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
	return S_OK;
}

CLoadBone* CLoadBone::Create(const aiNode* pAINode, _int iParentIndex)
{
	CLoadBone* instance = new CLoadBone();
	if (FAILED(instance->Initialize(pAINode, iParentIndex))) {
		Safe_Release(instance);
	}
	return instance;
}

void CLoadBone::Free()
{
	__super::Free();
}
