#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_PickUp :
    public CPlayerState
{
private:
    CPlayerState_PickUp();
    virtual ~CPlayerState_PickUp() DEFAULT;

public:
    virtual void OnEnter();
    virtual void OnUpdate(_float dt);
    virtual void OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();

public:
    static CPlayerState_PickUp* Create();
    virtual void Free();
};
NS_END
