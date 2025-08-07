#pragma once
#include "CBase.h"
BEGIN(Engine)

class ENGINE_DLL CStateCache :
	public CBase
{
private:
	explicit CStateCache();
	virtual ~CStateCache();

public:
	static CStateCache* Create(LPDIRECT3DDEVICE9 pDevice);
public:
	void SetRenderState(_D3DRENDERSTATETYPE type, DWORD value);
	void SetFVF(DWORD value);
	void Clear();
private:
	HRESULT Ready_Cache(LPDIRECT3DDEVICE9 pDevice);
private:
	LPDIRECT3DDEVICE9 m_pDevice;
	unordered_map<_D3DRENDERSTATETYPE, DWORD> m_stateContainer;
	DWORD curFVF;
private:
	void Free();
};

END
