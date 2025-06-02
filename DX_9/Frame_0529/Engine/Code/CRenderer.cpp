#include "Engine_Define.h"
#include "CRenderer.h"
#include "CMesh.h"
#include "CTransform.h"
#include "CGameObject.h"

CRenderer::CRenderer()
	:m_pMesh(nullptr),m_pTransform(nullptr)
{
}

CRenderer::~CRenderer()
{
}

CRenderer* CRenderer::Create()
{
	CRenderer* instance = new CRenderer;

	if (FAILED(instance->Ready_Component())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}


HRESULT CRenderer::Ready_Component()
{

	return S_OK;
}

void CRenderer::Update_Component(float dt)
{
}

void CRenderer::LateUpdate_Component(float dt)
{
}

void CRenderer::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if (!m_pMesh || !m_pTransform) return;
	pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());

	const auto& vtx = m_pMesh->Get_VertexBuffer();
	const auto& idx = m_pMesh->Get_IndexBuffer();
	
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	HRESULT hr = pDevice->CreateVertexBuffer(
		sizeof(VTXCOL) * vtx.size(), //버퍼 사이즈(개수)
		0, // 
		FVF_COL,
		D3DPOOL_MANAGED,
		&pVB,
		nullptr
	);

	if (FAILED(hr)) return;

	void* pVtxData = nullptr;
	pVB->Lock(0, 0, &pVtxData, 0);
	memcpy(pVtxData, vtx.data(), sizeof(VTXCOL) * vtx.size());
	pVB->Unlock();

	LPDIRECT3DINDEXBUFFER9 pIB = nullptr;
	hr = pDevice->CreateIndexBuffer(
		sizeof(INDEX16) * idx.size(),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pIB,
		nullptr
	);

	if (FAILED(hr)) return;

	void* pIdxData = nullptr;
	pIB->Lock(0, 0, &pIdxData, 0);
	memcpy(pIdxData, idx.data(), sizeof(INDEX16) * idx.size());
	pIB->Unlock();

	pDevice->SetStreamSource(0, pVB, 0, sizeof(VTXCOL));
	pDevice->SetFVF(FVF_COL);
	pDevice->SetIndices(pIB);

	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,              // BaseVertexIndex
		0,              // MinVertexIndex
		vtx.size(),     // NumVertices
		0,              // StartIndex
		idx.size()  // PrimitiveCount (삼각형 개수)
	);

	pVB->Release();
	pIB->Release();
}


CComponent* CRenderer::Clone() const
{
	return nullptr;
}

void CRenderer::Set_Mesh()
{
	m_pMesh = m_pOwner->Get_Component<CMesh>();

	if (m_pMesh == nullptr)
		return ;

	m_pTransform = m_pOwner->Get_Component<CTransform>();

	if (m_pTransform == nullptr)
		return ;
}

void CRenderer::Free()
{
}
