#include "AIBone.h"

CAIBone::CAIBone()
{
}

HRESULT CAIBone::Initialize(const aiNode* pAINode, _int iParentIndex)
{
	m_iParentBoneIndex = iParentIndex;
	m_BoneName = pAINode->mName.data;
	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));

	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	return S_OK;
}

void CAIBone::Save_File(ofstream& ofs)
{
	BONE_INFO_HEADER infoHeader = {};
	strcpy_s(infoHeader.BoneName, sizeof(infoHeader.BoneName), m_BoneName.data());
	infoHeader.TransformationMatrix = { m_TransformationMatrix };
	infoHeader.ParentBoneIndex = { m_iParentBoneIndex };
	ofs.write(reinterpret_cast<const char*>(&infoHeader), sizeof(BONE_INFO_HEADER));
}

CAIBone* CAIBone::Create(const aiNode* pAINode, _int iParentIndex)
{
	CAIBone* instance = new CAIBone();
	if (FAILED(instance->Initialize(pAINode, iParentIndex))) {
		Safe_Release(instance);
	}
	return instance;
}

void CAIBone::Free()
{
	__super::Free();
}
