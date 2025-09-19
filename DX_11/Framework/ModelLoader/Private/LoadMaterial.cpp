#include "Loader_Defines.h"
#include "LoadMaterial.h"
#include "Helper_Func.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "GameObject.h"
#include "Texture.h"
#include "AIMaterial.h"

CLoadMaterial::CLoadMaterial()
	:m_pDevice(CGameInstance::GetInstance()->Get_Device()),
	m_pContext(CGameInstance::GetInstance()->Get_Context())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CLoadMaterial::CLoadMaterial(const CLoadMaterial& rhs)
	:CMaterial(rhs), m_pDevice(rhs.m_pDevice), m_pContext(rhs.m_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CLoadMaterial::Initialize()
{

	return S_OK;
}

void CLoadMaterial::Render_GUI()
{
	__super::Render_GUI();
}

HRESULT CLoadMaterial::Load_Material(_uint materialNum, aiMaterial* material[], const string& filePath)
{
	m_MaterialKey = Helper::GetFileNameWithOutExtension(filePath);
	string Directory = filesystem::path(filePath).parent_path().string();

	for (size_t i = 0; i < materialNum; i++)
	{
		CAIMaterial* data = CAIMaterial::Create(m_pDevice, material[i], string(m_MaterialKey + to_string(i)), Directory);
		if (data) {
			m_MaterialDatas.push_back(data);
		}
	}
	return S_OK;
}

HRESULT CLoadMaterial::Save_Material()
{
	string path = Helper::SaveFileDialogByWinAPI(m_MaterialKey, "mat");
	filesystem::path directory(path);
	ofstream ofs(path.c_str(), ios::binary);
	if (!ofs.is_open())
		return E_FAIL;

	MATERIAL_FILE_HEADER fileHead = {};
	strcpy_s(fileHead.materialDataKey, sizeof(fileHead.materialDataKey), m_MaterialKey.c_str());
	fileHead.MaterialDataCount = m_MaterialDatas.size();
	ofs.write(reinterpret_cast<const char*>(&fileHead), sizeof(fileHead));

	for (size_t i = 0; i < m_MaterialDatas.size(); i++)
		dynamic_cast<CAIMaterial*>(m_MaterialDatas[i])->Save_MaterialData(m_pContext,ofs, directory.parent_path().string());

	ofs.close();
}

void CLoadMaterial::LinkShader(const string& shader)
{
	for (CMaterialData* pMdata : m_MaterialDatas) {
		static_cast<CAIMaterial*>(pMdata)->LinkShader(shader);
	}
}

CLoadMaterial* CLoadMaterial::Create()
{
	CLoadMaterial* instance = new CLoadMaterial;
	if (FAILED(instance->Initialize_Prototype())) {
		Safe_Release(instance);
	}
	return instance;
}

CComponent* CLoadMaterial::Clone()
{
	CLoadMaterial* instance = new CLoadMaterial(*this);
	return instance;
}

void CLoadMaterial::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
