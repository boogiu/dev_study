#pragma once
#include "Level.h"

NS_BEGIN(Engine)
class CGameInstance;
class CUI_Object;
class IProtoService;
class IObjectService;
NS_END


NS_BEGIN(Client)

class CLogoLevel final :
    public CLevel
{
private:
    CLogoLevel(const string& LevelKey);
    virtual ~CLogoLevel() DEFAULT;

public:
    virtual HRESULT Initialize() override;
    virtual HRESULT Awake() override;
    virtual void Update() override;
    virtual HRESULT Render()override;

public:
    static void PreLoad_Level();

private:
    CGameInstance* m_pGameInstance = { nullptr };
    _bool m_bStart = false;
public:
    static CLogoLevel* Create(const string& LevelKey);
    virtual void Free() override;

 
};

NS_END