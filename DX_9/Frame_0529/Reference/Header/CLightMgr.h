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
    void Update_Light(_float& dt);
    void Set_Priority();
    void Add_Light(CLight* light);

private:
    LPDIRECT3DDEVICE9 m_pDevice;
    vector<CLight*> m_LightContainer;
    LIGHT_POLICY m_ePolicy;

private:
    void Free() override;
};

END