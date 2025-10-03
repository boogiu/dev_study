#include "AIBone.h"

CAIBone::CAIBone()
{
}

HRESULT CAIBone::Initialize(const aiNode* pAINode, _int iParentIndex)
{
	m_iParentBoneIndex = iParentIndex;
	m_BoneName = pAINode->mName.C_Str();
	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));


	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	if (iParentIndex == -1) {
		_matrix		PreTransformMatrix = XMMatrixIdentity();
		PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(g_iImportPreRotate));
		XMStoreFloat4x4(&m_TransformationMatrix,
			PreTransformMatrix* XMLoadFloat4x4(&m_TransformationMatrix));
	}

	return S_OK;
}

void CAIBone::Save_File(ofstream& ofs)
{
	BONE_INFO_HEADER infoHeader = {};
	strcpy_s(infoHeader.BoneName, sizeof(infoHeader.BoneName), m_BoneName.data());

	_matrix SaveMatrix = XMLoadFloat4x4(&m_TransformationMatrix);

	if (m_iParentBoneIndex == -1) {
		_matrix		PreTransformMatrix = XMMatrixIdentity();
		PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(g_iExportPreRotate));
		SaveMatrix = PreTransformMatrix* SaveMatrix;
	}
	XMStoreFloat4x4(&infoHeader.TransformationMatrix, SaveMatrix);
	infoHeader.ParentBoneIndex = { m_iParentBoneIndex };
	ofs.write(reinterpret_cast<const char*>(&infoHeader), sizeof(BONE_INFO_HEADER));
}

void CAIBone::Render_GUI()
{
	
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
