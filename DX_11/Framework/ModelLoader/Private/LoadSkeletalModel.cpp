#include "Loader_Defines.h"
#include "LoadSkeletalModel.h"
#include "GameInstance.h"
#include "IGraphicService.h"
#include "Helper_Func.h"
#include "AIModelData.h"

CLoadSkeletalModel::CLoadSkeletalModel()
{
}

CLoadSkeletalModel::CLoadSkeletalModel(const CLoadSkeletalModel& Prototype)
	:CSkeletalModel(Prototype), m_pDevice{ Prototype.m_pDevice }
{
	Safe_AddRef(m_pDevice);
}

HRESULT CLoadSkeletalModel::Initialize_Prototype()
{
	m_pDevice = CGameInstance::GetInstance()->Get_Device();
	if (!m_pDevice)
		return E_FAIL;
	Safe_AddRef(m_pDevice);

	return S_OK;
}

HRESULT CLoadSkeletalModel::Initialize(COMPONENT_DESC* pArg)
{
	return S_OK;
}

HRESULT CLoadSkeletalModel::Load_Model(const aiScene* pAIScene, const string& fileName)
{
	Release_Mesh();
	_uint meshNum = pAIScene->mNumMeshes;
	m_DrawableMeshes.resize(meshNum, true);
	m_pData = CAIModelData::Create(pAIScene, m_pDevice, MESH_TYPE::ANIM);
	m_fileName = fileName;
	_float4x4 IdentityMatrix;
	XMStoreFloat4x4(&IdentityMatrix, XMMatrixIdentity());

	m_TransfromationMatrices.resize(m_pData->Get_BoneCount(), IdentityMatrix);
	m_CombinedMatrices.resize(m_pData->Get_BoneCount(), IdentityMatrix);
	m_FinalMatices.resize(m_pData->Get_BoneCount(), IdentityMatrix);

	for (size_t i = 0; i < m_pData->Get_BoneCount(); i++)
	{
		int parent = m_pData->Get_BoneParentIndex(i);

		if (parent == -1) {
			m_CombinedMatrices[i] = m_TransfromationMatrices[i];
		}
		else {
			_matrix ParentCombine = XMLoadFloat4x4(&m_CombinedMatrices[parent]);
			_matrix MyTransformation = XMLoadFloat4x4(&m_TransfromationMatrices[i]);
			XMStoreFloat4x4(&m_CombinedMatrices[i], MyTransformation * ParentCombine);
		}
	}

	for (size_t i = 0; i < m_pData->Get_BoneCount(); i++)
	{
		XMStoreFloat4x4(&m_FinalMatices[i], m_pData->Get_OffsetMatrix(i) * XMLoadFloat4x4(&m_CombinedMatrices[i]));
	}

	return S_OK;
}

HRESULT CLoadSkeletalModel::Save_Model()
{
	string path = Helper::SaveFileDialogByWinAPI(m_fileName, "model");
	filesystem::path directory(path);
	ofstream ofs(path.c_str(), ios::binary);
	if (!ofs.is_open())
		return E_FAIL;
	
	MODEL_FILE_HEADER fileHeader = {};
	fileHeader.isAnimate = true;
	fileHeader.MeshCount = m_pData->Get_MeshCount();
	strcpy_s(fileHeader.ModelKey, sizeof(fileHeader.ModelKey), m_fileName.data());
	ofs.write(reinterpret_cast<char*>(&fileHeader), sizeof(MODEL_FILE_HEADER));

	static_cast<CAIModelData*>(m_pData)->Save_File(ofs);
	ofs.close();
	return S_OK;
}

HRESULT CLoadSkeletalModel::Save_Model(const string& SavePath)
{
	filesystem::path folderPath = filesystem::path(SavePath) / m_fileName;

	filesystem::create_directories(folderPath);

	filesystem::path fullSavePath = folderPath / (m_fileName + ".model");

	ofstream ofs(fullSavePath.c_str(), ios::binary);
	if (!ofs.is_open())
		return E_FAIL;

	MODEL_FILE_HEADER fileHeader = {};
	fileHeader.isAnimate = false;
	fileHeader.MeshCount = m_pData->Get_MeshCount();
	strcpy_s(fileHeader.ModelKey, sizeof(fileHeader.ModelKey), m_fileName.data());
	ofs.write(reinterpret_cast<char*>(&fileHeader), sizeof(MODEL_FILE_HEADER));

	static_cast<CAIModelData*>(m_pData)->Save_File(ofs);
	ofs.close();
	return S_OK;
}

void CLoadSkeletalModel::Render_GUI()
{
	__super::Render_GUI();
	string ID = "HideMesh : ";
	for (size_t i = 0; i < m_pData->Get_MeshCount(); i++)
	{

		if (ImGui::Button((ID + to_string(i)).c_str()))
		{
			if (m_DrawableMeshes.size() >= 1) {
				m_DrawableMeshes[i] = !m_DrawableMeshes[i];
			}
		}
	}
	
}

HRESULT CLoadSkeletalModel::Release_Mesh()
{
	Safe_Release(m_pData);
	vector<bool> v;
	m_DrawableMeshes.swap(v);

	return S_OK;
}

CLoadSkeletalModel* CLoadSkeletalModel::Create()
{
	CLoadSkeletalModel* instance = new CLoadSkeletalModel();
	if (FAILED(instance->Initialize_Prototype()))
	{
		Safe_Release(instance);
	}
	return instance;
}

CComponent* CLoadSkeletalModel::Clone()
{
	return new CLoadSkeletalModel(*this);
}

void CLoadSkeletalModel::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
}