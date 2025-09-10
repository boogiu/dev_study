#include "VI_Terrain.h"

CVI_Terrain::CVI_Terrain(const string& key)
	:CVIBuffer{ key }
{
}

CVI_Terrain::CVI_Terrain(const CVI_Terrain& rhs)
	: CVIBuffer(rhs)
{
}

CVI_Terrain::~CVI_Terrain()
{
}

HRESULT CVI_Terrain::Initialize(ID3D11Device* pDevice, const string& HeightfilePath)
{
	_ulong dwByte = {};
	HANDLE handle = CreateFileA(HeightfilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == handle)
		return E_FAIL;

	BITMAPFILEHEADER fHeader;
	ReadFile(handle, &fHeader, sizeof(fHeader), &dwByte, nullptr);
	if (dwByte != sizeof(fHeader)) return E_FAIL;
	BITMAPINFOHEADER iHeader;
	ReadFile(handle, &iHeader, sizeof(iHeader), &dwByte, nullptr);
	if (dwByte != sizeof(iHeader)) return E_FAIL;

	_uint iBmpSize = iHeader.biWidth * iHeader.biHeight;

	pPixels = new _uint[iBmpSize];
	ReadFile(handle, pPixels, sizeof(_uint) * iBmpSize, &dwByte, nullptr);
	CloseHandle(handle);

	iVerticesCountX = iHeader.biWidth;
	iVerticesCountZ = iHeader.biHeight;


	m_iVertexBufferCount = 1;
	m_iVerticesCount = iVerticesCountX * iVerticesCountZ;
	m_iVertexStride = sizeof(VTXNORMTEX);

	m_iIndicesCount = (iVerticesCountX - 1) * (iVerticesCountZ - 1) * 3 * 2;
	m_iIndexStride = 4; //byte
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	Create_Vertex(pDevice);

	Safe_Delete_Array(pPixels);
	return S_OK;
}

HRESULT CVI_Terrain::Create_Vertex(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC VBDesc;
	VBDesc.ByteWidth = m_iVertexStride * m_iVerticesCount;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = m_iVertexStride;

	VB = new VTXNORMTEX[m_iVerticesCount];
	ZeroMemory(VB, m_iVertexStride * m_iVerticesCount);

	_float s = 0.5;

	for (size_t i = 0; i < iVerticesCountZ; ++i) {
		for (size_t j = 0; j < iVerticesCountX; ++j) {
			_uint       iIndex = i * iVerticesCountX + j;
			/*ARGB -> 합연산 B만 쳐버리면, B의 마지막 인자 00000000(8비트)중 켜진 것만 가져오니까*/
			VB[iIndex].vPosition = _float3(j, (pPixels[iIndex] & 0x000000ff) / 10.f, i);
			VB[iIndex].vTexcoord = _float2(j / (iVerticesCountX - 1.f), (i / (iVerticesCountX - 1.f)));
			VB[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
		}
	}

	Create_Index(pDevice);

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = VB;
	HRESULT hr = pDevice->CreateBuffer(&VBDesc, &subData, &m_pVB);
	Safe_Delete_Array(VB);
	return hr;
}

HRESULT CVI_Terrain::Create_Index(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC IDDesc;
	IDDesc.ByteWidth = m_iIndexStride * m_iIndicesCount;
	IDDesc.Usage = D3D11_USAGE_DEFAULT;
	IDDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IDDesc.CPUAccessFlags = 0;
	IDDesc.MiscFlags = 0;
	IDDesc.StructureByteStride = m_iIndexStride;

	IB = new _uint[m_iIndicesCount];
	ZeroMemory(IB, m_iIndexStride * m_iIndicesCount);
	_uint iCount = {};

	//0~128까지.
	for (size_t i = 0; i < iVerticesCountZ-1; ++i) {
		for (size_t j = 0; j < iVerticesCountX-1; ++j) {

			_uint iIndex = j + (iVerticesCountX)*i;

			_uint       iIndices[4] = {
				/*0*/			iIndex + iVerticesCountX,
				/*1*/			iIndex + iVerticesCountX + 1,
				/*2*/			iIndex + 1,
				/*3*/			iIndex
			};

			IB[iCount++] = iIndices[0];
			IB[iCount++] = iIndices[1];
			IB[iCount++] = iIndices[2];
			IB[iCount++] = iIndices[0];
			IB[iCount++] = iIndices[2];
			IB[iCount++] = iIndices[3];

		}
	}

	ComputeNormal();

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = IB;

	HRESULT hr = pDevice->CreateBuffer(&IDDesc, &subData, &m_pIB);

	Safe_Delete_Array(IB);
	return hr;
}


void CVI_Terrain::ComputeNormal()
{
	for (size_t i = 0; i < m_iIndicesCount; i += 3) {
		if (i  >= 98304)
			return;
		//면의 법선의 누적
		_uint index0 = IB[i];
		_uint index1 = IB[i + 1];
		_uint index2 = IB[i + 2];

		_vector vPosition0 = XMLoadFloat3(&VB[index0].vPosition);
		_vector vPosition1 = XMLoadFloat3(&VB[index1].vPosition);
		_vector vPosition2 = XMLoadFloat3(&VB[index2].vPosition);

		_vector vNormal0 = XMLoadFloat3(&VB[index0].vNormal);
		_vector vNormal1 = XMLoadFloat3(&VB[index1].vNormal);
		_vector vNormal2 = XMLoadFloat3(&VB[index2].vNormal);

		_vector vEdge01 = vPosition1 - vPosition0;		//(0이 1을 바라봄)
		_vector vEdge02 = vPosition2 - vPosition0;	//(0이 2를 바라봄)

		_vector vPlaneNorm = XMVector3Normalize(XMVector3Cross(vEdge01, vEdge02));
		vNormal0 += vPlaneNorm;
		vNormal1 += vPlaneNorm;
		vNormal2 += vPlaneNorm;

		XMStoreFloat3(&VB[index0].vNormal, vNormal0);
		XMStoreFloat3(&VB[index1].vNormal, vNormal1);
		XMStoreFloat3(&VB[index2].vNormal, vNormal2);
	}

	for (size_t i = 0; i < m_iVerticesCount; ++i) {
		_vector vNormal = XMLoadFloat3(&VB[i].vNormal);
		XMStoreFloat3(&VB[i].vNormal,XMVector3Normalize(vNormal));
	}

}

CVI_Terrain* CVI_Terrain::Create(ID3D11Device* pDevice, const string& key, const string& HeightfilePath)
{
	CVI_Terrain* instance = new CVI_Terrain(key);
	if (FAILED(instance->Initialize(pDevice, HeightfilePath))) {
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(instance);
	}
	return instance;
}

void CVI_Terrain::Free()
{
	__super::Free();
}
