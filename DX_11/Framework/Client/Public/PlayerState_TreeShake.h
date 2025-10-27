#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_TreeShake :
    public CPlayerState
{
private:
    CPlayerState_TreeShake();
    virtual ~CPlayerState_TreeShake() DEFAULT;

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
    static CPlayerState_TreeShake* Create();
    virtual void Free();
};
NS_END
