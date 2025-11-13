#include "AIModelData.h"
#include "AIMesh.h"
#include "AISkeleton.h"
CAIModelData::CAIModelData()
{
}

CAIModelData::~CAIModelData()
{
}

HRESULT CAIModelData::Initialize(const aiScene* pAIScene, ID3D11Device* pDevice, MESH_TYPE eType)
{
	_uint meshNum = pAIScene->mNumMeshes;
	m_pSkeleton = CAISkeleton::Create(pAIScene->mRootNode);
	aiMatrix4x4 identity;
	CollectMeshNodes(pAIScene->mRootNode, identity);

	for (size_t i = 0; i < meshNum; i++)
	{
		CAIMesh* pMesh = CAIMesh::Create(pDevice, pAIScene->mMeshes[i], m_pSkeleton,  eType);
		if (nullptr == pMesh)
			return E_FAIL;
		m_Meshes.push_back(pMesh);
	}
	return S_OK;
}

void CAIModelData::CollectMeshNodes(aiNode* pNode, const aiMatrix4x4& parentTransform)
{
	aiMatrix4x4 current = parentTransform * pNode->mTransformation;

	// 이 노드가 포함한 메쉬 정보 기록
	for (UINT i = 0; i < pNode->mNumMeshes; i++)
	{
		UINT meshIndex = pNode->mMeshes[i];
		m_MeshNodeTransform[meshIndex] = current;
		m_MeshNodeName[meshIndex] = pNode->mName.C_Str();
	}

	// 자식 노드 반복
	for (UINT c = 0; c < pNode->mNumChildren; c++)
	{
		CollectMeshNodes(pNode->mChildren[c], current);
	}
}

void CAIModelData::Render_GUI()
{
	if (ImGui::Button("Bones Tab")) {
		m_bShowSkeletonBones = !m_bShowSkeletonBones;
	}
	string boneCount = "Bone : "+ to_string(m_pSkeleton->Get_BoneCount());

	ImGui::Text(boneCount.c_str());

	for (auto& mesh : m_Meshes) {
		mesh->Render_GUI();
	}

	if (m_bShowSkeletonBones) {
		ImGui::SetNextWindowSize(ImVec2(500, 400));
		if (ImGui::Begin("SkeletonBones", &m_bShowSkeletonBones, ImGuiWindowFlags_NoCollapse))
		{
			m_pSkeleton->Render_GUI();
		}
		ImGui::End();
	}
}

void CAIModelData::Save_File(ofstream& ofs)
{

	for (size_t i = 0; i < m_Meshes.size(); i++)
	{
		static_cast<CAIMesh*>(m_Meshes[i])->Save_File(ofs);
	}

	static_cast<CAISkeleton*>(m_pSkeleton)->Save_File(ofs);
}

CAIModelData* CAIModelData::Create(const aiScene* pAIScene, ID3D11Device* pDevice, MESH_TYPE eType)
{
	CAIModelData* instance = new CAIModelData;
	if (FAILED(instance->Initialize(pAIScene, pDevice,eType))) {
		Safe_Release(instance);
	}
	return instance;
}

void CAIModelData::Free()
{
	__super::Free();
}