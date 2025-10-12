#include "AISkeleton.h"
#include "AIBone.h"

CAISkeleton::CAISkeleton()
{
}

HRESULT CAISkeleton::Ready_Bones(const aiNode* pAINode, _int iParentIndex)
{
	CAIBone* pBone = CAIBone::Create(pAINode, iParentIndex);
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);

	_int iPIndex = m_Bones.size() - 1;

	for (_uint i = 0; i < pAINode->mNumChildren; ++i)
	{
		Ready_Bones(pAINode->mChildren[i], iPIndex);
	}

	m_BoneMap.emplace(pAINode->mName.C_Str(), iPIndex);

	_float4x4 IdentityMat;
	XMStoreFloat4x4(&IdentityMat, XMMatrixIdentity());
	m_OffsetMatrices.resize(m_Bones.size(), IdentityMat);

	return S_OK;
}

void CAISkeleton::Set_Offset(_uint Index, _float4x4 offset)
{
	if (m_Bones[Index]->Get_ParentIndex() == -1) {
		_matrix PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(g_iImportPreRotate)); 
		_matrix Offset = XMLoadFloat4x4(&offset);
		Offset = PreTransformMatrix * Offset;
		XMStoreFloat4x4(&m_OffsetMatrices[Index], Offset);
	}
	else {
		m_OffsetMatrices[Index] = offset;
	}
	//m_OffsetMatrices[Index] = offset;
}

HRESULT CAISkeleton::Save_Skeletons(const string& path)
{
	return S_OK;
}

void CAISkeleton::Save_File(ofstream& ofs)
{
	SKELETON_FILE_HEADER skeleton = {};
	skeleton.BoneCount = m_Bones.size();
	ofs.write(reinterpret_cast<const char*>(&skeleton), sizeof(SKELETON_FILE_HEADER));

	for (size_t i = 0; i < m_Bones.size(); i++)
	{
		static_cast<CAIBone*>(m_Bones[i])->Save_File(ofs);
	}

	_matrix		PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(g_iExportPreRotate));

	for (size_t i = 0; i < m_OffsetMatrices.size(); i++)
	{
		_float4x4 SaveFloat4x4 = {};
		_matrix SaveMatrix = XMLoadFloat4x4(&m_OffsetMatrices[i]);
		XMStoreFloat4x4(&SaveFloat4x4, SaveMatrix);
		ofs.write(reinterpret_cast<const char*>(&SaveFloat4x4), sizeof(_float4x4));
	}

}


CAISkeleton* CAISkeleton::Create(const aiNode* pAINode)
{
	CAISkeleton* instance = new CAISkeleton;
	if (FAILED(instance->Ready_Bones(pAINode, -1))) {
		Safe_Release(instance);
	}
	return instance;
}

void CAISkeleton::Free()
{
	__super::Free();
}
