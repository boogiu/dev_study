#pragma once
#include "Level.h"
NS_BEGIN(Engine)
class CGameInstance;
class IProtoService;
class IObjectService;
NS_END
NS_BEGIN(Client)

class CLoadingLevel :
    public CLevel
{
private:
    CLoadingLevel(const string& LevelKey);
    virtual ~CLoadingLevel() override;

public:
    virtual HRESULT Initialize() override;
    virtual HRESULT Awake() override;
    virtual void Update() override;
    virtual HRESULT Render()override;

private:
    class CLoader* m_pLoader = { nullptr };
    _float m_fLoadingTime = {};
public:
    static CLoadingLevel* Create(const string& LevelKey);
    virtual void Free() override;
};

NS_END