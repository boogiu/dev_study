#include "LoadAnimatedModel.h"
#include "GameInstance.h"
#include "IGraphicService.h"
#include "Helper_Func.h"
#include "AIModelData.h"
CLoadAnimatedModel::CLoadAnimatedModel()
{
}

CLoadAnimatedModel::CLoadAnimatedModel(const CLoadAnimatedModel& Prototype)
	:CAnimatedModel(Prototype), m_pDevice{ Prototype.m_pDevice }
{
	Safe_AddRef(m_pDevice);
}

HRESULT CLoadAnimatedModel::Initialize_Prototype()
{
	m_pDevice = CGameInstance::GetInstance()->Get_Device();
	if (!m_pDevice)
		return E_FAIL;
	Safe_AddRef(m_pDevice);

	return S_OK;
}

HRESULT CLoadAnimatedModel::Initialize(COMPONENT_DESC* pArg)
{
	return S_OK;
}

HRESULT CLoadAnimatedModel::Load_Model(const aiScene* pAIScene, const string& fileName)
{
	Release_Mesh();
	_uint meshNum = pAIScene->mNumMeshes;
	m_DrawableMeshes.resize(meshNum, true);
	m_pData = CAIModelData::Create(pAIScene, m_pDevice, MESH_TYPE::ANIM);
	m_fileName = fileName;
	return S_OK;
}

HRESULT CLoadAnimatedModel::Save_Model()
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

HRESULT CLoadAnimatedModel::Release_Mesh()
{
	Safe_Release(m_pData);
	vector<bool> v;
	m_DrawableMeshes.swap(v);

	return S_OK;
}

CLoadAnimatedModel* CLoadAnimatedModel::Create()
{
	CLoadAnimatedModel* instance = new CLoadAnimatedModel();
	if (FAILED(instance->Initialize_Prototype()))
	{
		Safe_Release(instance);
	}
	return instance;
}

CComponent* CLoadAnimatedModel::Clone()
{
	return new CLoadAnimatedModel(*this);
}

void CLoadAnimatedModel::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
}