#pragma once
#include "Npc_HubState.h"
NS_BEGIN(Client)
class CNpcStateHub_Move :
    public CNpc_HubState
{
private:
    CNpcStateHub_Move();
    virtual ~CNpcStateHub_Move() DEFAULT;

public:
    HRESULT Initialize();
public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

    void DecideSubState(_float dt) override;

public:
    void Render_State() override;
private:
    _float m_fMoveTime = {};
public:
    static CNpcStateHub_Move* Create();
    virtual void Free();
};

NS_END