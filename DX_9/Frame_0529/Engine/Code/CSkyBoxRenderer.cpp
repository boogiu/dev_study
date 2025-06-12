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
	m_pMesh = CResourceMgr::GetInstance()->Find_Mesh("Deafult_Cube");
	
	if (m_pDevice&& m_pMesh) {
		m_pDevice->AddRef();
		return S_OK;
	}

	return E_FAIL;
}
void CSkyBoxRenderer::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if (!m_pTransform || !pDevice)
		return;

	// 2. 월드 행렬은 카메라 위치로만 구성된 변환 행렬
	pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);           // 안쪽 면 보이게
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                 // Z-비활성화
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);

	pDevice->SetStreamSource(0, m_pMesh->GetVertexBuffer(), 0, sizeof(VTXLIGHTTEX));
	pDevice->SetFVF(m_pMesh->GetFVF());
	pDevice->SetIndices(m_pMesh->GetIndexBuffer());
	pDevice->SetTexture(0, m_pTexture);

	HRESULT hr;
	hr = m_pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		8,
		0,
		36);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           // 기본 컬링 복구
	if (FAILED(hr))
		return;
}

CComponent* CSkyBoxRenderer::Clone() const
{
	return nullptr;
}

void CSkyBoxRenderer::Set_Texture(const string& _key)
{

	const UINT texWidth = 128;
	const UINT texHeight = 128;

	// 1. 텍스처 생성
	HRESULT hr = D3DXCreateTexture(
		m_pDevice,
		texWidth,
		texHeight,
		1,                     // MipLevels
		0,                     // Usage
		D3DFMT_A8R8G8B8,       // Format
		D3DPOOL_MANAGED,
		&m_pTexture
	);

	if (FAILED(hr) || !m_pTexture)
	{
		MessageBoxA(nullptr, "텍스처 생성 실패", "Error", MB_OK);
		return;
	}

	// 2. 텍스처 락 걸기
	D3DLOCKED_RECT lockedRect;
	if (SUCCEEDED(m_pTexture->LockRect(0, &lockedRect, nullptr, 0)))
	{
		DWORD* pixels = reinterpret_cast<DWORD*>(lockedRect.pBits);

		for (UINT y = 0; y < texHeight; ++y)
		{
			for (UINT x = 0; x < texWidth; ++x)
			{
				// 예: 그라디언트 (빨강 + 파랑)
				BYTE red = static_cast<BYTE>((x / (float)texWidth) * 255);
				BYTE blue = static_cast<BYTE>((y / (float)texHeight) * 255);
				pixels[y * (lockedRect.Pitch / 4) + x] = D3DCOLOR_ARGB(255, red, 0, blue);
			}
		}

		m_pTexture->UnlockRect(0);
	}

}


void CSkyBoxRenderer::Free()
{
	Safe_Release(m_pMesh);
	Safe_Release(m_pDevice);
}