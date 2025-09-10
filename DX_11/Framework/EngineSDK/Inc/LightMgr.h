#pragma once
#include "ILightService.h"
NS_BEGIN(Engine)
class CLightMgr :
    public ILightService
{
private:
    CLightMgr();
    virtual ~CLightMgr();

public:
    HRESULT Initialize();
    virtual void Register_Light(LIGHT_DESC* Light) override;
    virtual void UnRegister_Light(LIGHT_DESC* Light) override;

private:
    vector<LIGHT_DESC*> m_Lights;

public:
    static CLightMgr* Create();
    virtual void Free() override;
};

NS_END