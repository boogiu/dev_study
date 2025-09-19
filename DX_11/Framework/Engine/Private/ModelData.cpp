#include "ModelData.h"
#include "Mesh.h"
#include "Skeleton.h"

CModelData::CModelData()
{
}

CModelData::~CModelData()
{
}

HRESULT CModelData::Initialize(const string& filePath, ID3D11Device* pDevice)
{
	ifstream ifs(filePath.c_str(), ios::binary);
	if (!ifs.is_open()) {
		MSG_BOX("There is No File. :CModelData ");
		return E_FAIL;
	}

	MODEL_FILE_HEADER fileHeader = {};
	ifs.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

	for (int i = 0; i < fileHeader.MeshCount; ++i) {
		CMesh* newMesh = CMesh::Create(pDevice, ifs, fileHeader.isAnimate? MESH_TYPE::ANIM : MESH_TYPE::NONANIM);
		if (newMesh)
			m_Meshes.push_back(newMesh);
		else
		{
			ifs.close();
			for (auto& mesh : m_Meshes)
				Safe_Release(mesh);
			return E_FAIL;
		}
	}


	if (fileHeader.isAnimate) {
		m_pSkeleton = CSkeleton::Create(ifs);
	}

	ifs.close();

	return S_OK;
}

_uint CModelData::Get_MaterialIndex(_uint meshIndex)
{
	return m_Meshes[meshIndex]->Get_MaterialIndex();
}

_uint CModelData::Get_BoneCount()
{
	return m_pSkeleton->Get_BoneCount();
}

_int CModelData::Find_BoneIndexByName(const string& BoneName)
{
	return m_pSkeleton->Find_BoneIndexByName(BoneName);
}

const string& CModelData::Find_BoneNameByIndex(_uint BoneIndex)
{
	return m_pSkeleton->Find_BoneNameByIndex(BoneIndex);
}

const D3D11_INPUT_ELEMENT_DESC* CModelData::Get_ElementDesc(_uint DrawIndex)
{
	return m_Meshes[DrawIndex]->Get_ElementDesc();
}

const _uint CModelData::Get_ElementCount(_uint DrawIndex)
{
	return m_Meshes[DrawIndex]->Get_ElementCount();
}

const string_view CModelData::Get_ElementKey(_uint DrawIndex)
{
	return m_Meshes[DrawIndex]->Get_ElementKey();
}

void CModelData::Render_GUI()
{
	
}

HRESULT CModelData::Render_Mesh(ID3D11DeviceContext* pContext, _uint Index)
{
	if (Index >= m_Meshes.size()) return E_FAIL;

	m_Meshes[Index]->Bind_Buffer(pContext);
	m_Meshes[Index]->Render(pContext);

	return S_OK;
}

_matrix CModelData::Get_OffsetMatrix(_uint BoneIndex)
{
	return m_pSkeleton->Get_OffsetMatrix(BoneIndex);
}


_int CModelData::Get_BoneParentIndex(_uint i)
{
	return m_pSkeleton->Get_BoneParentIndex(i);
}

_float4x4 CModelData::Get_TransformMatrix(_uint BoneIndex)
{
	return m_pSkeleton->Get_TransformationMatrix(BoneIndex);
}

CModelData* CModelData::Create(const string& filePath, ID3D11Device* pDevice)
{
	CModelData* instance = new CModelData;

	if (FAILED(instance->Initialize(filePath, pDevice))) {
		Safe_Release(instance);
	}
	return instance;
}

void CModelData::Free()
{
	__super::Free();
	Safe_Release(m_pSkeleton);
	for (auto& mesh : m_Meshes)
		Safe_Release(mesh);
}
