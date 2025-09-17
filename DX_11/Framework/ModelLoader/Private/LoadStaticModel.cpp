#include "LoadStaticModel.h"
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "IGraphicService.h"
#include "LoadedStaticMesh.h"
#include "Helper_Func.h"
#include "LoadBone.h"
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

HRESULT CLoadStaticModel::Load_Model(_uint meshNum, aiMesh* mesh[], const string& filePath)
{
	Release_Mesh();
	fileName = Helper::GetFileNameWithOutExtension(filePath);
	for (size_t i = 0; i < meshNum; i++)
	{
		CLoadedStaticMesh* pMesh = CLoadedStaticMesh::Create(m_pDevice, mesh[i], string(fileName + to_string(i)));
		if (nullptr == pMesh)
			return E_FAIL;

		m_Buffers.push_back(pMesh);
		m_DrawableMeshes.push_back(true);
	}
}

HRESULT CLoadStaticModel::Save_Model()
{
	string path = Helper::SaveFileDialogByWinAPI(fileName, "model");
	ofstream ofs(path.c_str(), ios::binary);
	if (!ofs.is_open())
		return E_FAIL;

	MESH_FILE_HEADER fileHead = {};
	strcpy_s(fileHead.meshKey, sizeof(fileHead.meshKey), fileName.c_str());
	fileHead.MeshCount = m_Buffers.size();

	ofs.write(reinterpret_cast<const char*>(&fileHead), sizeof(fileHead));

	for (CMesh* buffer : m_Buffers)
	{
		CLoadedStaticMesh* mesh = static_cast<CLoadedStaticMesh*>(buffer);
		const vector<VTXMESH> vertices = mesh->Get_Vertex();
		size_t vSize = vertices.size();
		const vector<_uint> indices = mesh->Get_Index();
		size_t iSize = indices.size();

		MESH_INFO_HEADER infoHead = {};
		infoHead.VerticesCount = vertices.size();
		infoHead.IndicesCount = indices.size();
		infoHead.MaterialIndex = mesh->Get_MaterialIndex();
		infoHead.isAnimate = false;
		infoHead.BoneCount = {};
		ofs.write(reinterpret_cast<const char*>(&infoHead), sizeof(infoHead));
		ofs.write(reinterpret_cast<const char*>(vertices.data()), vertices.size() * sizeof(VTXMESH));
		ofs.write(reinterpret_cast<const char*>(indices.data()), indices.size() * sizeof(_uint));
	}

	ofs.close();

	return S_OK;
}

HRESULT CLoadStaticModel::Release_Mesh()
{
	for (auto& mesh : m_Buffers)
		Safe_Release(mesh);

	m_Buffers.clear();

	vector<_bool>v = {};
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
