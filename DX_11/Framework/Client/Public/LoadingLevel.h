#pragma once
#include "Level.h"

NS_BEGIN(Client)

class CLoadingLevel :
    public CLevel
{
private:
    CLoadingLevel(const string& LevelKey);
    virtual ~CLoadingLevel() override;

public:
    virtual HRESULT Initialize() override;
    virtual void Update() override;
    virtual HRESULT Render()override;

private:
    class CLoader* m_pLoader = { nullptr };

public:
    static CLoadingLevel* Create(const string& LevelKey);
    virtual void Free() override;
};

NS_END