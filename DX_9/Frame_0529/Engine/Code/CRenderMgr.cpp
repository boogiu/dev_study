#include "Engine_Define.h"
#include "CRenderMgr.h"
#include "CCameraMgr.h"
#include "CRenderer.h"

IMPLEMENT_SINGLETON(CRenderMgr)

CRenderMgr::CRenderMgr()
{
}

CRenderMgr::~CRenderMgr()
{
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
	for (auto& renderer : m_Renderers)
		renderer->Render(pDevice);
}

void CRenderMgr::Clear()
{
}

void CRenderMgr::Free()
{
}
