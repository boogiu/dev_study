#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Transfer :
    public CPlayerState
{
private:
    CPlayerState_Transfer();
    virtual ~CPlayerState_Transfer() DEFAULT;

public:
    virtual void OnEnter();
    virtual void OnUpdate(_float dt);
    virtual void OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();

public:
    static CPlayerState_Transfer* Create();
    virtual void Free();
};

NS_END