#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_ShakeTree :
    public CPlayerState
{
private:
    CPlayerState_ShakeTree();
    virtual ~CPlayerState_ShakeTree() DEFAULT;

public:
    virtual void OnEnter();
    virtual void OnUpdate(_float dt);
    virtual void OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();

private:
    _float KeepTime = { 0.f };
    _bool isLooped = { false };
    _bool isFinished = { false };
public:
    static CPlayerState_ShakeTree* Create();
    virtual void Free();
};
NS_END
