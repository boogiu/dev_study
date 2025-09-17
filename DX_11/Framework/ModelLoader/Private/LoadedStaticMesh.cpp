#include "LoadedStaticMesh.h"
#include "Engine_Defines.h"

CLoadedStaticMesh::CLoadedStaticMesh(const string& meshID)
	:CMesh(meshID)
{
}

CLoadedStaticMesh::~CLoadedStaticMesh()
{
}

HRESULT CLoadedStaticMesh::Initialize(ID3D11Device* pDevice, const aiMesh* pAIMesh)
{
	m_iVertexBufferCount = 1;
	m_iVerticesCount = pAIMesh->mNumVertices;
	m_iVertexStride = sizeof(VTXMESH);

	m_iIndicesCount = pAIMesh->mNumFaces * 3;
	m_iIndexStride = 4;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pAIMesh = pAIMesh;
	m_MaterialIndex = pAIMesh->mMaterialIndex;

	Create_Vertex(pDevice);
	Create_Index(pDevice);
	return S_OK;
}

HRESULT CLoadedStaticMesh::Create_Vertex(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC VBDesc;
	VBDesc.ByteWidth = m_iVertexStride * m_iVerticesCount;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = m_iVertexStride;

	m_VBContainer.reserve(m_iVerticesCount);

    for (size_t i = 0; i < m_iVerticesCount; i++)
    {
		VTXMESH mesh = {};
        memcpy(&mesh.vPosition,		&m_pAIMesh->mVertices[i], sizeof(_float3));
        memcpy(&mesh.vNormal,			&m_pAIMesh->mNormals[i], sizeof(_float3));
        memcpy(&mesh.vTexcoord,	&m_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
        memcpy(&mesh.vTangent,		&m_pAIMesh->mTangents[i], sizeof(_float3));
		m_VBContainer.push_back(mesh);
    }

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = m_VBContainer.data();
	HRESULT hr = pDevice->CreateBuffer(&VBDesc, &subData, &m_pVB);

	return hr;
}

HRESULT CLoadedStaticMesh::Create_Index(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC IDDesc;
	IDDesc.ByteWidth = m_iIndexStride * m_iIndicesCount;
	IDDesc.Usage = D3D11_USAGE_DEFAULT;
	IDDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IDDesc.CPUAccessFlags = 0;
	IDDesc.MiscFlags = 0;
	IDDesc.StructureByteStride = m_iIndexStride;

	m_IBContainer.reserve(m_iIndicesCount);


	for (size_t i = 0; i < m_pAIMesh->mNumFaces; i++)
	{
		m_IBContainer.push_back(m_pAIMesh->mFaces[i].mIndices[0]);
		m_IBContainer.push_back(m_pAIMesh->mFaces[i].mIndices[1]);
		m_IBContainer.push_back(m_pAIMesh->mFaces[i].mIndices[2]);
	}

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = m_IBContainer.data();

	HRESULT hr = pDevice->CreateBuffer(&IDDesc, &subData, &m_pIB);

	return hr;
}

CLoadedStaticMesh* CLoadedStaticMesh::Create(ID3D11Device* pDevice, const aiMesh* pAIMesh, const string& meshID)
{
	CLoadedStaticMesh* instance = new CLoadedStaticMesh(meshID);
	if (FAILED(instance->Initialize(pDevice, pAIMesh))) {
		Safe_Release(instance);
	}

	instance->m_pAIMesh = { nullptr };
	return instance;
}

void CLoadedStaticMesh::Free()
{
	m_VBContainer.clear();
	m_IBContainer.clear();
	__super::Free();
}

