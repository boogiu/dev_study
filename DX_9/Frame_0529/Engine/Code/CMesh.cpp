#include "Engine_Define.h"
#include "CMesh.h"
#include "CGraphicDev.h"

CMesh::CMesh()
	:m_dwSubsetCnt(0), 
	m_FVF(FVF_LIGHTTEX),
	m_pMesh(nullptr),
	m_Key{},
	m_pDevice(nullptr)
{}

CMesh::~CMesh()
{
}

CMesh* CMesh::Create()
{
	CMesh* instance = new CMesh;

	if (FAILED(instance->Ready_Mesh())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CMesh::Ready_Mesh()
{
	m_pDevice = CGraphicDev::GetInstance()->Get_GraphicDev();
	
	if (!m_pDevice)
		return E_FAIL;

	m_pDevice->AddRef();
	
	return S_OK;
}
void CMesh::SetMesh(LPD3DXMESH mesh)
{
	if (m_pMesh) //기존 메시 해제
		m_pMesh->Release();

	if (!mesh)
	{
		m_pMesh = nullptr;
		return;
	}

	// 원하는 FVF: 텍스처 좌표 포함
	m_pMesh = mesh;
	m_pMesh->AddRef();
	m_FVF = mesh->GetFVF();

	// 서브셋 정보 갱신
	if (m_pMesh)
	{
		m_pMesh->GetAttributeTable(nullptr, &m_dwSubsetCnt);
		m_Subset.resize(m_dwSubsetCnt);
		m_pMesh->GetAttributeTable(m_Subset.data(), &m_dwSubsetCnt);
	}
}


HRESULT CMesh::Load(const string pFile)
{
	return S_OK;
}

void CMesh::Free()
{
	if (m_pDevice) m_pDevice->Release();
	if (m_pMesh) m_pMesh->Release();
}

void CMesh::Debug_VertexUV()
{
	if (!m_pMesh)
		return;

	DWORD fvf = m_pMesh->GetFVF();
	if ((fvf & D3DFVF_TEX1) == 0)
	{
		OutputDebugStringA("❌ UV 좌표 없음 (D3DFVF_TEX1 미포함)\n");
		return;
	}

	DWORD vertexCount = m_pMesh->GetNumVertices();
	DWORD vertexSize = D3DXGetFVFVertexSize(fvf);

	void* pVertices = nullptr;
	if (FAILED(m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, &pVertices)))
	{
		OutputDebugStringA("❌ 버텍스 버퍼 잠금 실패\n");
		return;
	}

	// 정점 데이터 해석
	for (DWORD i = 0; i < vertexCount; ++i)
	{
		BYTE* pVtx = (BYTE*)pVertices + i * vertexSize;
		D3DXVECTOR3* pos = (D3DXVECTOR3*)pVtx;
		D3DXVECTOR3* normal = (D3DXVECTOR3*)(pVtx + sizeof(D3DXVECTOR3));
		float* uv = (float*)(pVtx + sizeof(D3DXVECTOR3) * 2);

		char buf[128];
		sprintf_s(buf, "[%d] Pos(%.2f, %.2f, %.2f)  UV(%.2f, %.2f)\n",
			i, pos->x, pos->y, pos->z, uv[0], uv[1]);
		OutputDebugStringA(buf);
	}

	m_pMesh->UnlockVertexBuffer();
}
