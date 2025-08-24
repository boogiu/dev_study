#pragma once
#include "Level.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CLogoLevel final :
    public CLevel
{
private:
    CLogoLevel();
    virtual ~CLogoLevel() DEFAULT;

public:
    virtual HRESULT Initialize() override;
    virtual void Update() override;
    virtual HRESULT Render()override;

private:
    CGameInstance* m_pGameInstance = { nullptr };
public:
    static CLogoLevel* Create();
    virtual void Free() override;

};

NS_END