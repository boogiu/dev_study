#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Interaction :
    public CPlayerState
{
private:
    CPlayerState_Interaction();
    virtual ~CPlayerState_Interaction() DEFAULT;

public:
    virtual void OnEnter();
    virtual void OnUpdate(_float dt);
    virtual void OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();

public:
    static CPlayerState_Interaction* Create();
    virtual void Free();
};

NS_END