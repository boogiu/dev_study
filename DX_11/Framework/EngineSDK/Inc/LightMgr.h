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
    virtual _int Register_Light(class CLight* Light) override;
    virtual void UnRegister_Light(_int ID) override;

private:
    vector<_bool> m_LightLifes;
    vector<class CLight*> m_Lights;

public:
    static CLightMgr* Create();
    virtual void Free() override;
};

NS_END