#include "Engine_Define.h"
#include "CTerrain.h"
#include "CGraphicDev.h"

CTerrain::CTerrain()
	:m_FVF(FVF_LIGHTTEX),
	m_pVB(nullptr),
	m_pIB(nullptr)
{
}

CTerrain::CTerrain(CTerrain& rhs)
	: m_FVF(rhs.m_FVF),
	m_pVB(rhs.m_pVB),
	m_pIB(rhs.m_pIB),
	m_pDevice(rhs.m_pDevice)
{
	m_key = rhs.m_key;
	m_heightMap = rhs.m_heightMap;
	m_iCol = rhs.m_iCol;
	m_iRow = rhs.m_iRow;

	if (m_pVB)     m_pVB->AddRef();
	if (m_pIB)     m_pIB->AddRef();
	if (m_pDevice) m_pDevice->AddRef();
}

CTerrain::~CTerrain()
{
}

CTerrain* CTerrain::Create()
{
	CTerrain* instance = new CTerrain;

	if (FAILED(instance->Ready_Mesh())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CTerrain::Ready_Mesh()
{
	m_pDevice = CGraphicDev::GetInstance()->Get_GraphicDev();
	if(m_pDevice)
		return S_OK;

	return E_FAIL;
}

void CTerrain::SetVTX_Buffer(const LPDIRECT3DVERTEXBUFFER9& vb)
{
	if (m_pVB)
		m_pVB->Release();

	m_pVB = vb;

	if(m_pVB)
		m_pVB->AddRef();
}

void CTerrain::SetIDX_Buffer(const LPDIRECT3DINDEXBUFFER9& ib)
{
	if (m_pIB)
		m_pIB->Release();

	m_pIB = ib;

	if(m_pIB)
		m_pIB->AddRef();
}

void CTerrain::Free()
{
	if (m_pDevice)
		m_pDevice->Release();

	if (m_pVB)
		m_pVB->Release();

	if (m_pIB)
		m_pIB->Release();
}

void CTerrain::ComputeIDX()
{

}

/// 지형 메시의 경우, 버텍스 버퍼가 지형 크기에 따라 달라질 수 있기에
/// 사용시 복사 받아 사용하는 형태로 진행
CTerrain* CTerrain::Clone()
{
	return new CTerrain(*this);
}

