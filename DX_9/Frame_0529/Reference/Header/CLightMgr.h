#pragma once
#include "CBase.h"

BEGIN(Engine)

class CLight;

class ENGINE_DLL CLightMgr :
    public CBase
{
    DECLARE_SINGLETON(CLightMgr)

private:
    explicit CLightMgr();
    virtual ~CLightMgr() override;

public :
    HRESULT Ready_Light(LPDIRECT3DDEVICE9 pDevice);
    void Set_Light(LPDIRECT3DDEVICE9 pDevice);
    void Add_Light(CLight* light);
    void Remove_Light(CLight* light);

private:
    D3DLIGHT9 m_baseLight;
    vector<CLight*> m_LightContainer;
    vector<bool> m_ActivedLight;

private:
    void Free() override;
};

END