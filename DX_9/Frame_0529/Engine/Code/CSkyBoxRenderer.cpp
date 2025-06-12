#include "Engine_Define.h"
#include "CSkyBoxRenderer.h"
#include "CCubeMesh.h"
#include "CResourceMgr.h"
#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CCamera.h"
#include "CCameraMgr.h"
#include "CTransform.h"


CSkyBoxRenderer::CSkyBoxRenderer()
	:m_pTexture(nullptr)
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
	HRESULT hr = Set_Buffer();
	if (m_pDevice && !FAILED(hr)) {
		m_pDevice->AddRef();
		return S_OK;
	}

	return E_FAIL;
}
void CSkyBoxRenderer::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if (!m_pTransform || !m_pVB || !m_pIB || !m_pTexture || !pDevice)
		return;

	// 2. 월드 행렬은 카메라 위치로만 구성된 변환 행렬
	pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());

	// 3. 렌더 상태 설정 (스카이박스용)
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);

	// 4. 텍스처 샘플링 상태 설정
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// 5. 정점/인덱스/텍스처 설정
	pDevice->SetTexture(0, m_pTexture);
	pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXTEX));
	pDevice->SetFVF(FVF_TEX);
	pDevice->SetIndices(m_pIB);

	// 6. 드로우 호출
	HRESULT hr = pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	// 7. 상태 복구
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "스카이박스 렌더링 실패", "Draw Error", MB_OK);
	}
}

CComponent* CSkyBoxRenderer::Clone() const
{
	return nullptr;
}

void CSkyBoxRenderer::Set_Texture(const string& _key)
{
	string path = "../Bin/Resource/SkyBox.dds";
	//wstring path = L"../Resource/SkyBox.dds";
	HRESULT hr = D3DXCreateTextureFromFile(m_pDevice, path.c_str(), &m_pTexture);
	//HRESULT hr = D3DXCreateCubeTextureFromFile(m_pDevice, path.c_str(), &m_pTexture);
	if (FAILED(hr))
	{
		MessageBoxA(0, "큐브 텍스처를 찾지 못했습니다", _key.c_str(), MB_OK);
		m_pTexture = nullptr;
		return;
	}
}

HRESULT CSkyBoxRenderer::Set_Buffer()
{
	const DWORD numIndices = 36;
	const DWORD numVertices = 8;

	HRESULT hr = m_pDevice->CreateVertexBuffer(sizeof(VTXTEX) * numVertices,
		0, FVF_TEX, D3DPOOL_MANAGED, &m_pVB, 0);

	// 인덱스 버퍼 생성
	HRESULT hr2 = m_pDevice->CreateIndexBuffer(sizeof(DWORD) * numIndices,
		0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, 0);

	if (FAILED(hr) || FAILED(hr2))
		return E_FAIL;

	VTXTEX cubeVertices[] = {
		{{-1,  1, -1}, {-1,  1}},
		{{ 1, -1, -1}, { 1, -1}},
		{{ 1, -1, -1}, { 1, -1}},
		{{-1, -1, -1}, {-1, -1}},

		{{-1,  1,  1}, {-1,  1}},
		{{ 1, -1,  1}, { 1, -1}},
		{{ 1, -1,  1}, { 1, -1}},
		{{-1, -1,  1}, {-1, -1}},
	};

	WORD cubeIndices[] = {
		3,0,1, 3,1,2,
		6,5,4, 6,4,7,
		2,1,5,2,5,6,
		7,4,0,7,0,3,
		0,4,5,0,5,1,
		7,3,2,7,2,6
	};

	// 3. 정점 버퍼에 복사
	void* pVertices = nullptr;
	m_pVB->Lock(0, 0, &pVertices, 0);
	memcpy(pVertices, cubeVertices, sizeof(cubeVertices));
	m_pVB->Unlock();

	// 6. 인덱스 버퍼에 복사
	void* pIndices = nullptr;
	m_pIB->Lock(0, 0, &pIndices, 0);
	memcpy(pIndices, cubeIndices, sizeof(cubeIndices));
	m_pIB->Unlock();

	return S_OK;
}

void CSkyBoxRenderer::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pDevice);
}