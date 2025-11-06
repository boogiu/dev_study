#pragma once
#include "NpcState.h"
NS_BEGIN(Client)
class CNpcState_Idle_Wait :
    public CNpcState
{
private:
    CNpcState_Idle_Wait();
    virtual ~CNpcState_Idle_Wait() DEFAULT;

public:
    virtual HRESULT OnEnter() override;
    virtual void OnUpdate(_float dt) override;
    virtual HRESULT OnExit() override;
    virtual CState* HandleTransition() override;

public:
    static CNpcState_Idle_Wait* Create();
    virtual void Free() override;
};
NS_END
