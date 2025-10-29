#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Net :
    public CPlayerState
{
private:
    CPlayerState_Net();
    virtual ~CPlayerState_Net() DEFAULT;

public:
    virtual void OnEnter();
    virtual void OnUpdate(_float dt);
    virtual void OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();

public:
    static CPlayerState_Net* Create();
    virtual void Free();
};
NS_END