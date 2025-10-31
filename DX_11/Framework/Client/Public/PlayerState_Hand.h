#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_HandAction :
    public CPlayerState
{
private:
    CPlayerState_HandAction();
    virtual ~CPlayerState_HandAction() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();

public:
    static CPlayerState_HandAction* Create();
    virtual void Free();
};
NS_END
