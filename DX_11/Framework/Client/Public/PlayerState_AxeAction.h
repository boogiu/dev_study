#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_AxeAction :
    public CPlayerState
{
private:
    CPlayerState_AxeAction();
    virtual ~CPlayerState_AxeAction() DEFAULT;

public:
    virtual void OnEnter();
    virtual void OnUpdate(_float dt);
    virtual void OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();

private:
    _float KeepTime = { 0.f };
    _bool isLooped = { false };
public:
    static CPlayerState_AxeAction* Create();
    virtual void Free();
};
NS_END
