#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Tool :
    public CPlayerState
{
private:
    CPlayerState_Tool();
    virtual ~CPlayerState_Tool() DEFAULT;

public:
    virtual void OnEnter();
    virtual void OnUpdate(_float dt);
    virtual void OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();

private:

public:
    static CPlayerState_Tool* Create();
    virtual void Free();
};
NS_END
