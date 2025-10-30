#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Walk :
    public CPlayerState
{
private:
    CPlayerState_Walk();
    virtual ~CPlayerState_Walk() DEFAULT;

public:
    virtual void OnEnter();
    virtual void OnUpdate(_float dt);
    virtual void OnExit();
    virtual CState* HandleTransition();
    virtual _uint Get_InputMask() const override;

private:
    _bool isFlipping = { false };
public:
    static CPlayerState_Walk* Create();
    virtual void Free();
};

NS_END