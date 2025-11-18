#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Pole :
    public CPlayerState
{
private:
    CPlayerState_Pole();
    virtual ~CPlayerState_Pole() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();

public:
    static CPlayerState_Pole* Create();
    virtual void Free();
};
NS_END