#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Repelled :
    public CPlayerState
{
private:
    CPlayerState_Repelled();
    virtual ~CPlayerState_Repelled() DEFAULT;

public:
    virtual void OnEnter();
    virtual void OnUpdate(_float dt);
    virtual void OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();

private:
    _bool NoTool = { false };
public:
    static CPlayerState_Repelled* Create();
    virtual void Free();
};
NS_END
