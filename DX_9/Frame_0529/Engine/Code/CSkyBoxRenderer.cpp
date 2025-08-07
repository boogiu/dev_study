#include "Engine_Define.h"
#include "CSkyBoxRenderer.h"
#include "CCubeMesh.h"
#include "CResourceMgr.h"
#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CCamera.h"
#include "CCameraMgr.h"
#include "CTransform.h"
#include "IMesh.h"
#include "CStateCache.h"


CSkyBoxRenderer::CSkyBoxRenderer()
{
}

CSkyBoxRenderer::~CSkyBoxRenderer()
{
}

CSkyBoxRenderer* CSkyBoxRenderer::Create()
{
	CSkyBoxRenderer* instance = new CSkyBoxRenderer;

	if (FAILED(instance->Ready_Component())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CSkyBoxRenderer::Ready_Component()
{
	m_pDevice = CGraphicDev::GetInstance()->Get_GraphicDev();
	
	if (m_pDevice) {
		m_pDevice->AddRef();
		return S_OK;
	}

	return E_FAIL;
}

void CSkyBoxRenderer::Update_Component(float& dt)
{
}

void CSkyBoxRenderer::LateUpdate_Component(float& dt)
{
	if (m_bActive)
		CRenderMgr::GetInstance()->Add_Renderer(this);
}

void CSkyBoxRenderer::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if (!m_pTransform || !pDevice)
		return;
	pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());

	m_pCache->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pCache->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pCache->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	m_pCache->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// 스카이박스 렌더링 전
	m_pCache->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pCache->SetFVF(m_pCube->GetFVF());

	pDevice->SetStreamSource(0, m_pCube->GetVertexBuffer(), 0, sizeof(VTXCUBE));
	pDevice->SetTexture(0, m_cubeTexture);
	pDevice->SetIndices(m_pCube->GetIndexBuffer());
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12 );

	m_pCache->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pCache->SetRenderState(D3DRS_ZENABLE, TRUE);

	m_pCache->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pCache->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	m_pCache->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           // 기본 컬링 복구
}

CComponent* CSkyBoxRenderer::Clone() const
{
	return nullptr;
}

void CSkyBoxRenderer::Set_SkyBox(const wstring& _key)
{
	m_pCube = CResourceMgr::GetInstance()->Find_Mesh(L"Deafult_Cube");
	m_pCube->AddRef();
	const wstring texBasePath = L"../Bin/Resource/Textures/" + _key;
	D3DXCreateCubeTextureFromFileW(m_pDevice,texBasePath.c_str(), &m_cubeTexture);
}


void CSkyBoxRenderer::Free()
{
	Safe_Release(m_pCube);
	Safe_Release(m_cubeTexture);
	Safe_Release(m_pDevice);
}