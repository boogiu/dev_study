#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_ToolBase :
    public CPlayerState
{
private:
    CPlayerState_ToolBase();
    virtual ~CPlayerState_ToolBase() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();

private:
    _float KeepTime = { 0.f };
    _bool isLooped = { false };

public:
    static CPlayerState_ToolBase* Create();
    virtual void Free();
};
NS_END