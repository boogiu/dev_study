#include "LoadStaticModel.h"
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "IGraphicService.h"
#include "Helper_Func.h"
#include "AIModelData.h"
CLoadStaticModel::CLoadStaticModel()
{
}

CLoadStaticModel::CLoadStaticModel(const CLoadStaticModel& Prototype)
	:CStaticModel(Prototype),m_pDevice{ Prototype .m_pDevice}
{
	Safe_AddRef(m_pDevice);
}

HRESULT CLoadStaticModel::Initialize_Prototype()
{
	m_pDevice = CGameInstance::GetInstance()->Get_Device();
	if (!m_pDevice)
		return E_FAIL;
	Safe_AddRef(m_pDevice);

	return S_OK;
}

HRESULT CLoadStaticModel::Initialize(COMPONENT_DESC* pArg)
{

	return S_OK;
}

void CLoadStaticModel::Render_GUI()
{
	ImGui::SeparatorText("Loaded Model");
}

HRESULT CLoadStaticModel::Load_Model(const aiScene* pAiScene, const string& fileName)
{
	Release_Mesh();
	_uint meshNum = pAiScene->mNumMeshes;
	m_DrawableMeshes.resize(meshNum, true);
	m_pData = CAIModelData::Create(pAiScene, m_pDevice, MESH_TYPE::NONANIM);
	m_fileName = fileName;
	return S_OK;
}

HRESULT CLoadStaticModel::Save_Model()
{
	string path = Helper::SaveFileDialogByWinAPI(m_fileName, "model");
	filesystem::path directory(path);
	ofstream ofs(path.c_str(), ios::binary);
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

HRESULT CLoadStaticModel::Release_Mesh()
{
	Safe_Release(m_pData);
	vector<bool> v;
	m_DrawableMeshes.swap(v);

	return S_OK;
}

CLoadStaticModel* CLoadStaticModel::Create()
{
	CLoadStaticModel* instance = new CLoadStaticModel();
	if (FAILED(instance->Initialize_Prototype()))
	{
		Safe_Release(instance);
	}
	return instance;
}

CComponent* CLoadStaticModel::Clone()
{
	return new CLoadStaticModel(*this);
}

void CLoadStaticModel::Free()
{
	__super::Free();
	Release_Mesh();
	Safe_Release(m_pDevice);
}
