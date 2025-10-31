#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Idle :
    public CPlayerState
{
private:
    CPlayerState_Idle();
    virtual ~CPlayerState_Idle() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

public:
    static CPlayerState_Idle* Create();
    virtual void Free();
};

NS_END