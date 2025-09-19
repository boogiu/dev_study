#include "Mesh.h"
#include "Bone.h"

CMesh::CMesh()
	: CVIBuffer("")
{
}
CMesh::CMesh(const string& ModelKey)
	:CVIBuffer(ModelKey)
{
}

CMesh::~CMesh()
{
}

HRESULT CMesh::Initialize_From_File(ID3D11Device* pDevice, ifstream& ifs, MESH_TYPE eType)
{
	MESH_INFO_HEADER infoHeader = {};


	ifs.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
	m_VIKey = infoHeader.MeshName;
	m_MaterialIndex = infoHeader.MaterialIndex;
	m_iVertexBufferCount = 1;
	m_iVerticesCount = infoHeader.VerticesCount;
	m_iVertexStride = eType == MESH_TYPE::ANIM  ? sizeof(VTXSKINMESH) : sizeof(VTXMESH);
	m_iIndicesCount = infoHeader.IndicesCount;
	m_iIndexStride = 4; //byte
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_ElementCount = eType == MESH_TYPE::ANIM ? VTXSKINMESH::iElementCount : VTXMESH::iElementCount;
	m_ElementKey = eType == MESH_TYPE::ANIM ? VTXSKINMESH::Key : VTXMESH::Key;
	m_ElementDesc = eType == MESH_TYPE::ANIM ? VTXSKINMESH::Elements : VTXMESH::Elements;
	HRESULT hr = eType==MESH_TYPE::ANIM ? Create_AnimateVertex(pDevice, ifs) : Create_StaticVertex(pDevice,  ifs);

	if (FAILED(hr))
		return E_FAIL;

	m_indices.resize(infoHeader.IndicesCount);
	ifs.read(reinterpret_cast<char*>(m_indices.data()), infoHeader.IndicesCount * sizeof(m_iIndexStride));

	if (FAILED(Create_Index(pDevice)))
		return E_FAIL;
	return S_OK;
}

HRESULT CMesh::Create_AnimateVertex(ID3D11Device* pDevice, ifstream& ifs)
{
	m_ElementCount = VTXSKINMESH::iElementCount;
	m_ElementKey = VTXSKINMESH::Key;
	m_ElementDesc = VTXSKINMESH::Elements;

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
	m_ElementCount = VTXMESH::iElementCount;
	m_ElementKey = VTXMESH::Key;
	m_ElementDesc = VTXMESH::Elements;
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

void CMesh::Render_GUI()
{
	
}


CMesh* CMesh::Create(ID3D11Device* pDevice, ifstream& ifs, MESH_TYPE eType)
{
	CMesh* instance = new CMesh();
	if (FAILED(instance->Initialize_From_File(pDevice, ifs, eType))) {
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
