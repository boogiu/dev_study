#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_Air :
    public CPlayerState
{
private:
    CPlayerState_Air();
    virtual ~CPlayerState_Air() DEFAULT;

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
    static CPlayerState_Air* Create();
    virtual void Free();
};
NS_END
