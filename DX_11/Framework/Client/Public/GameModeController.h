#pragma once
#include "LevelObject.h"

NS_BEGIN(Client)
class CGameModeController :
    public CLevelObject
{
private:
    CGameModeController();
    CGameModeController(const CGameModeController& rhs);
    ~CGameModeController() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Awake() override;
    void Priority_Update(_float dt)override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void Set_GameMode(GAME_MODE mode);
    void Get_GameMode();

private:
    GAME_MODE m_eMode = {  };

public:
    static CGameModeController* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
