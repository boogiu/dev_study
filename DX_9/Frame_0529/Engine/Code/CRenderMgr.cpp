#include "Engine_Define.h"
#include "CRenderMgr.h"
#include "CCameraMgr.h"
#include "CRenderer.h"
#include "CMeshRenderer.h"

IMPLEMENT_SINGLETON(CRenderMgr)

CRenderMgr::CRenderMgr()
{
}

CRenderMgr::~CRenderMgr()
{
	Free();
}

HRESULT CRenderMgr::Ready_RenderMgr(LPDIRECT3DDEVICE9 pDevice)
{
	return S_OK;
}

void CRenderMgr::Add_Renderer(CRenderer* renderer)
{
	auto iter = find_if(m_Renderers.begin(), m_Renderers.end(),
		[&renderer](CRenderer* data)->bool {
			return data == renderer;
		});

	if(iter == m_Renderers.end())
		m_Renderers.push_back(renderer);
}

void CRenderMgr::Remove_Renderer(CRenderer* renderer)
{
	auto iter = remove_if(m_Renderers.begin(), m_Renderers.end(),
		[&renderer](CRenderer* data)->bool {
			return data == renderer;
		});
	
	if (iter != m_Renderers.end()) {
		m_Renderers.erase(iter, m_Renderers.end());
	}
}

void CRenderMgr::Render(LPDIRECT3DDEVICE9 pDevice)
{

	//렌더 스테이트 설정
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//텍스쳐 설정
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	for (auto& renderer : m_Renderers)
		renderer->Render(pDevice);
}

void CRenderMgr::Clear()
{
}

void CRenderMgr::Free()
{
}
