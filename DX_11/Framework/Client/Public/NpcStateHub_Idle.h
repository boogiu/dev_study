#pragma once
#include "Npc_HubState.h"

NS_BEGIN(Client)
class CNpcStateHub_Idle :
    public CNpc_HubState
{
private:
    CNpcStateHub_Idle();
    virtual ~CNpcStateHub_Idle() DEFAULT;

public:
    HRESULT Initialize();
public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

public:
    void Render_State() override;
public:
    void DecideSubState(_float dt) override;

private:
    _float m_fIdleTime = {} ;
    _float m_fIdleDuration = {} ;
public:
    static CNpcStateHub_Idle* Create();
    virtual void Free();
};

NS_END