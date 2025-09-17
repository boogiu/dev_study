#include "LoadAnimatedModel.h"
#include "LoadedSkeleton.h"
#include "GameInstance.h"
#include "IGraphicService.h"
#include "LoadedAnimatedMesh.h"

#include "Helper_Func.h"

CLoadAnimatedModel::CLoadAnimatedModel()
{
}

CLoadAnimatedModel::CLoadAnimatedModel(const CLoadAnimatedModel& Prototype)
	:CAnimatedModel(Prototype), m_pDevice{ Prototype.m_pDevice }
{
	Safe_AddRef(m_pSkeleton);
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

void CLoadAnimatedModel::Render_GUI()
{
}

HRESULT CLoadAnimatedModel::Load_Model(const aiScene* pAIScene, const string& filePath)
{
	Release_Mesh();

	_uint meshNum = pAIScene->mNumMeshes;

	fileName = Helper::GetFileNameWithOutExtension(filePath);
	m_pSkeleton = CLoadedSkeleton::Create(pAIScene->mRootNode);

	for (size_t i = 0; i < meshNum; i++)
	{
		CLoadedAnimatedMesh* pMesh = CLoadedAnimatedMesh::Create(m_pDevice, pAIScene->mMeshes[i], m_pSkeleton);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Buffers.push_back(pMesh);
		m_DrawableMeshes.push_back(true);
	}

	m_FinalBoneMatrices.resize(m_Buffers.size());

	return S_OK;
}


HRESULT CLoadAnimatedModel::Save_Model()
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
		CLoadedAnimatedMesh* mesh = static_cast<CLoadedAnimatedMesh*>(buffer);
		const vector<VTXSKINMESH> vertices = mesh->Get_Vertex();
		size_t vSize = vertices.size();
		const vector<_uint> indices = mesh->Get_Index();
		size_t iSize = indices.size();

		MESH_INFO_HEADER infoHead = {};
		infoHead.VerticesCount = vertices.size();
		infoHead.IndicesCount = indices.size();
		infoHead.MaterialIndex = mesh->Get_MaterialIndex();
		infoHead.isAnimate = true;
		infoHead.BoneCount = buffer->Get_BoneCount();
		ofs.write(reinterpret_cast<const char*>(&infoHead), sizeof(infoHead));
		ofs.write(reinterpret_cast<const char*>(vertices.data()), vertices.size() * sizeof(VTXSKINMESH));
		ofs.write(reinterpret_cast<const char*>(indices.data()), indices.size() * sizeof(_uint));

		ofs.write(reinterpret_cast<const char*>(mesh->Get_BoneIndices().data()), mesh->Get_BoneIndices().size() * sizeof(_uint));
		ofs.write(reinterpret_cast<const char*>(mesh->Get_Offset().data()), mesh->Get_Offset().size() * sizeof(_float4x4));
	}

	ofs.close();

	return S_OK;
}

HRESULT CLoadAnimatedModel::Release_Mesh()
{
	for (auto& mesh : m_Buffers)
		Safe_Release(mesh);

	m_Buffers.clear();
	m_FinalBoneMatrices.clear();

	vector<_bool>v = {};
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
	Safe_Release(m_pSkeleton);
	
	__super::Free();
	Release_Mesh();
	Safe_Release(m_pDevice);
}