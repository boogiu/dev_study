#pragma once
#include "CBase.h"

BEGIN(Engine)
class CRenderer;
class 	CStateCache;
class CFont;

class ENGINE_DLL CRenderMgr :
    public CBase
{
    DECLARE_SINGLETON(CRenderMgr)

private:
	explicit CRenderMgr();
	virtual ~CRenderMgr();

public:
	HRESULT Ready_RenderMgr(LPDIRECT3DDEVICE9 pDevice);
	void Render(LPDIRECT3DDEVICE9 pDevice);
public:
	CStateCache* Get_Cache() { return m_StateCache; }
	void Add_Renderer(CRenderer* renderer);
	void Remove_Renderer(CRenderer* renderer);
	void Clear();

private:
	unordered_map<RENDER_PASS, list<CRenderer*>> m_Renderers;
	CStateCache* m_StateCache;

private:
	virtual void Free() override;
};

END