#pragma once
#include "StateMachine.h"
#include "NonPlayer.h"

NS_BEGIN(Client)

class CNpcState_Machine :
    public CStateMachine
{
private:
    CNpcState_Machine(CNonPlayer* pNpc);
    virtual ~CNpcState_Machine() DEFAULT;

public:
    HRESULT Initialize();

public:
    void Update(_float dt) override;
    virtual void Request_ChangeState(STATE_LAYER eLayer, const string& NextState) override;

public:
    void Render_State(class CNonPlayer* pPlayer);

private:
    CNonPlayer* m_pOwner = { nullptr };

public:
    static CNpcState_Machine* Create(CNonPlayer* pPlayer);
    virtual void Free();
};

NS_END