#pragma once
#include "Level.h"

NS_BEGIN(Client)

class CLoadingLevel :
    public CLevel
{
private:
    CLoadingLevel();
    virtual ~CLoadingLevel() override;

public:
    virtual HRESULT Initialize();
    virtual void Update();
    virtual HRESULT Render();

private:
    class CLoader* m_pLoader = { nullptr };

public:
    static CLoadingLevel* Create();
    virtual void Free() override;
};

NS_END