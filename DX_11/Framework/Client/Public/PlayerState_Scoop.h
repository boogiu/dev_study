#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Scoop :
    public CPlayerState
{
private:
    CPlayerState_Scoop();
    virtual ~CPlayerState_Scoop() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();

public:
    static CPlayerState_Scoop* Create();
    virtual void Free();
};
NS_END