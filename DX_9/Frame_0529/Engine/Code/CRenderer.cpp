#include "Engine_Define.h"
#include "CRenderer.h"
#include "CMesh.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CTextureMgr.h"
#include "CCameraMgr.h"

CRenderer::CRenderer()
	:m_pMesh(nullptr), m_pTransform(nullptr)
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
	//렌더 스테이트 설정
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); 

	if (m_pMesh->isSky()) {
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	}
	//텍스쳐 설정
	LPDIRECT3DTEXTURE9 Tex = CTextureMgr::GetInstance()->Get_Texture(m_pMesh->Get_Key());
	pDevice->SetTexture(0, Tex);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	//메쉬 재질
	pDevice->SetMaterial(&m_pMesh->Get_Material());

	const auto& vtx = m_pMesh->Get_VertexBuffer();

	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	HRESULT hr = pDevice->CreateVertexBuffer(
		sizeof(VTXLIGHTTEX) * vtx.size(), //버퍼 사이즈(개수)
		0, // 
		FVF_LIGHTTEX,
		D3DPOOL_MANAGED,
		&pVB,
		nullptr
	);

	if (FAILED(hr)) return;

	void* pVtxData = nullptr;

	pVB->Lock(0, 0, &pVtxData, 0);
		memcpy(pVtxData, vtx.data(), sizeof(VTXLIGHTTEX) * vtx.size());
	pVB->Unlock();

	//출력
	pDevice->SetStreamSource(0, pVB, 0, sizeof(VTXLIGHTTEX));
	pDevice->SetFVF(FVF_LIGHTTEX);
	pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vtx.size() / 3);

	pVB->Release();

#pragma region 인덱스 버퍼 사용시
	//const auto& idx = m_pMesh->Get_IndexBuffer();
	//LPDIRECT3DINDEXBUFFER9 pIB = nullptr;
	//hr = pDevice->CreateIndexBuffer(
	//	sizeof(INDEX16) * idx.size(),
	//	0,
	//	D3DFMT_INDEX16,
	//	D3DPOOL_MANAGED,
	//	&pIB,
	//	nullptr
	//);
	//
	//if (FAILED(hr)) return;
	//
	//void* pIdxData = nullptr;
	//pIB->Lock(0, 0, &pIdxData, 0);
	//memcpy(pIdxData, idx.data(), sizeof(INDEX16) * idx.size());
	//pIB->Unlock();

	//pDevice->SetIndices(pIB);
	//pDevice->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLELIST,
	//	0,              // BaseVertexIndex
	//	0,              // MinVertexIndex
	//	vtx.size(),     // NumVertices
	//	0,              // StartIndex
	//	idx.size()  // PrimitiveCount (삼각형 개수)
	//);


	//pIB->Release();
#pragma endregion

}


CComponent* CRenderer::Clone() const
{
	return nullptr;
}

void CRenderer::Set_Mesh()
{
	m_pMesh = m_pOwner->Get_Component<CMesh>();

	if (m_pMesh == nullptr)
		return;

	m_pTransform = m_pOwner->Get_Component<CTransform>();

	if (m_pTransform == nullptr)
		return;
}

void CRenderer::Free()
{
}
