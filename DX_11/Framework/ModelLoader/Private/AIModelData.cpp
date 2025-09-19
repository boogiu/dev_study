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

	for (size_t i = 0; i < meshNum; i++)
	{
		CAIMesh* pMesh = CAIMesh::Create(pDevice, pAIScene->mMeshes[i], m_pSkeleton,  eType);
		if (nullptr == pMesh)
			return E_FAIL;
		m_Meshes.push_back(pMesh);
	}
	return S_OK;
}

void CAIModelData::Render_GUI()
{
	if (ImGui::Button("ShowSkeletonBones")) {
		m_bShowSkeletonBones = !m_bShowSkeletonBones;
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
	static_cast<CAISkeleton*>(m_pSkeleton)->Save_File(ofs);

	for (size_t i = 0; i < m_Meshes.size(); i++)
	{
		static_cast<CAIMesh*>(m_Meshes[i])->Save_File(ofs);
	}
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