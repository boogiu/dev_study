#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_ReleaseTool :
    public CPlayerState
{
private:
    CPlayerState_ReleaseTool();
    virtual ~CPlayerState_ReleaseTool() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();

private:
    _bool m_bTransed = {false};
public:
    static CPlayerState_ReleaseTool* Create();
    virtual void Free();
};
NS_END