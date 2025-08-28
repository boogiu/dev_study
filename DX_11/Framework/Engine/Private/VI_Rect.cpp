#include "VI_Rect.h"

CVI_Rect::CVI_Rect()
{
}

CVI_Rect::CVI_Rect(const CVI_Rect& rhs)
	: CVIBuffer(rhs)
{
}

CVI_Rect::~CVI_Rect()
{
}

HRESULT CVI_Rect::Initialize(ID3D11Device* pDevice)
{
	m_iVertexBufferCount = 1;
	m_iVerticesCount = 4;
	m_iVertexStride = sizeof(VTXTEX);
	m_iIndicesCount = 6;
	m_iIndexStride = 2; //byte
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	if (FAILED(Create_Vertex(pDevice)))
		return E_FAIL;

	if (FAILED(Create_Index(pDevice)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVI_Rect::Create_Vertex(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC VBDesc;
	VBDesc.ByteWidth = m_iVertexStride * m_iVerticesCount;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = m_iVertexStride;

	VTXTEX* VB = new VTXTEX[m_iVerticesCount];
	ZeroMemory(VB, m_iVertexStride * m_iVerticesCount);

	_float s = 0.5;

	VB[0].vPosition = _float3(-s, s, 0.f);
	VB[0].vTexcoord = _float2(0.f, 0.f);

	VB[1].vPosition = _float3(s, s, 0.f);
	VB[1].vTexcoord = _float2(1.f, 0.f);

	VB[2].vPosition = _float3(s, -s, 0.f);
	VB[2].vTexcoord = _float2(1.f, 1.f);

	VB[3].vPosition = _float3(-s, -s, 0.f);
	VB[3].vTexcoord = _float2(0.f, 1.f);

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = VB;

	HRESULT hr = pDevice->CreateBuffer(&VBDesc, &subData, &m_pVB);

	Safe_Delete_Array(VB);
	return hr;
}

HRESULT CVI_Rect::Create_Index(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC IDDesc;
	IDDesc.ByteWidth = m_iIndexStride * m_iIndicesCount;
	IDDesc.Usage = D3D11_USAGE_DEFAULT;
	IDDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IDDesc.CPUAccessFlags = 0;
	IDDesc.MiscFlags = 0;
	IDDesc.StructureByteStride = m_iIndexStride;

	_ushort* IB = new _ushort[m_iIndicesCount];
	ZeroMemory(IB, m_iIndexStride * m_iIndicesCount);

	IB[0] = 0;    IB[1] = 1;    IB[2] = 2;    IB[3] = 0;    IB[4] = 2;    IB[5] = 3;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = IB;

	HRESULT hr = pDevice->CreateBuffer(&IDDesc, &subData, &m_pIB);

	Safe_Delete_Array(IB);
	return hr;
}

CVI_Rect* CVI_Rect::Create(ID3D11Device* pDevice)
{
	CVI_Rect* instance = new CVI_Rect();
	if (FAILED(instance->Initialize(pDevice))) {
		MSG_BOX("Failed to Created : CVIBuffer_Rect");
		Safe_Release(instance);
	}
	return instance;
}

void CVI_Rect::Free()
{
	__super::Free();
}
