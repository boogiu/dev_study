#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_NoTool :
    public CPlayerState
{
private:
    CPlayerState_NoTool();
    virtual ~CPlayerState_NoTool() DEFAULT;

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
    static CPlayerState_NoTool* Create();
    virtual void Free();
};
NS_END