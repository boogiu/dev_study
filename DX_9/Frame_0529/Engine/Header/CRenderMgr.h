#pragma once
#include "CBase.h"

BEGIN(Engine)
class CRenderer;

class ENGINE_DLL CRenderMgr :
    public CBase
{
    DECLARE_SINGLETON(CRenderMgr)

private:
	explicit CRenderMgr();
	virtual ~CRenderMgr();

public:
	void Add_Renderer(CRenderer* renderer);
	void Remove_Renderer(CRenderer* renderer);
	void Render(LPDIRECT3DDEVICE9 pDevice);
	void Clear();
private:
	vector<CRenderer*> m_Renderers;

private:
	virtual void Free() override;
};

END