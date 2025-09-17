#include "Mesh.h"
#include "Bone.h"
CMesh::CMesh(const string& meshKey)
	:CVIBuffer(meshKey)
{
}

CMesh::~CMesh()
{
}

HRESULT CMesh::Initialize_From_File(ID3D11Device* pDevice, ifstream& ifs)
{
	MESH_INFO_HEADER infoHeader = {};
	ifs.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
	m_MaterialIndex = infoHeader.MaterialIndex;
	m_iVertexBufferCount = 1;
	m_iVerticesCount = infoHeader.VerticesCount;
	m_iVertexStride = infoHeader.isAnimate? sizeof(VTXSKINMESH) : sizeof(VTXMESH);
	m_iIndicesCount = infoHeader.IndicesCount;
	m_iIndexStride = 4; //byte
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	HRESULT hr = infoHeader.isAnimate ? Create_AnimateVertex(pDevice, ifs) : Create_StaticVertex(pDevice,  ifs);

	if (FAILED(hr))
		return E_FAIL;

	m_indices.resize(infoHeader.IndicesCount);
	ifs.read(reinterpret_cast<char*>(m_indices.data()), infoHeader.IndicesCount * sizeof(m_iIndexStride));

	if (FAILED(Create_Index(pDevice)))
		return E_FAIL;

	ifs.read(reinterpret_cast<char*>(m_BoneIndices.data()), infoHeader.BoneCount * sizeof(_uint));
	ifs.read(reinterpret_cast<char*>(m_OffsetMatrices.data()), infoHeader.BoneCount * sizeof(_float4x4));

	return S_OK;
}

HRESULT CMesh::Create_AnimateVertex(ID3D11Device* pDevice, ifstream& ifs)
{
	vector<VTXSKINMESH>vertices = {};
	vertices.resize(m_iVerticesCount);

	ifs.read(reinterpret_cast<char*>(vertices.data()), m_iVerticesCount * sizeof(m_iVertexStride));

	D3D11_BUFFER_DESC VBDesc;
	VBDesc.ByteWidth = m_iVertexStride * m_iVerticesCount;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = m_iVertexStride;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = vertices.data();

	HRESULT hr = pDevice->CreateBuffer(&VBDesc, &subData, &m_pVB);
	return hr;
}

HRESULT CMesh::Create_StaticVertex(ID3D11Device* pDevice, ifstream& ifs)
{
	vector<VTXMESH>vertices = {};
	vertices.resize(m_iVerticesCount);

	ifs.read(reinterpret_cast<char*>(vertices.data()), m_iVerticesCount * sizeof(m_iVertexStride));

	D3D11_BUFFER_DESC VBDesc;
	VBDesc.ByteWidth = m_iVertexStride * m_iVerticesCount;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = m_iVertexStride;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = vertices.data();

	HRESULT hr = pDevice->CreateBuffer(&VBDesc, &subData, &m_pVB);
	return hr;
}


HRESULT CMesh::Create_Index(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC IDDesc;
	IDDesc.ByteWidth = m_iIndexStride * m_iIndicesCount;
	IDDesc.Usage = D3D11_USAGE_DEFAULT;
	IDDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IDDesc.CPUAccessFlags = 0;
	IDDesc.MiscFlags = 0;
	IDDesc.StructureByteStride = m_iIndexStride;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = m_indices.data();
	HRESULT hr = pDevice->CreateBuffer(&IDDesc, &subData, &m_pIB);

	vector<_uint>v = {};
	m_indices.swap(v);
	return hr;

}

_matrix CMesh::Get_BoneOffsetMatrix(_uint i)
{
	return XMLoadFloat4x4(&m_OffsetMatrices[i]);
}

_uint CMesh::Get_BoneIndex(_uint i)
{
	return m_BoneIndices[i];
}

const vector<_float4x4>& CMesh::Bind_BoneMatrices(const vector<CBone*>& Bones)
{
	m_BoneMatrices.resize(m_OffsetMatrices.size());
	for (size_t i = 0; i < m_OffsetMatrices.size(); i++)
	{
		XMStoreFloat4x4(&m_BoneMatrices[i],
			XMLoadFloat4x4(&m_OffsetMatrices[i]) *
			Bones[m_BoneIndices[i]]->Get_CombinedTransformationMatrix());
	}

	return m_BoneMatrices;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, const string& meshKey,ifstream& ifs)
{
	CMesh* instance = new CMesh(meshKey);
	if (FAILED(instance->Initialize_From_File(pDevice, ifs))) {
		Safe_Release(instance);
	}
	return instance;
}

void CMesh::Free()
{
	__super::Free();

	vector<_uint>i = {};
	m_indices.swap(i);
}
