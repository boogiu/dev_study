#pragma once
#include "NpcState.h"
NS_BEGIN(Client)
class CNpcState_Move_Trace :
    public CNpcState
{
private:
    CNpcState_Move_Trace();
    virtual ~CNpcState_Move_Trace() DEFAULT;

public:
    virtual HRESULT OnEnter() override;
    virtual void OnUpdate(_float dt) override;
    virtual HRESULT OnExit() override;
    virtual CState* HandleTransition() override;

private:
    //+
public:
    static CNpcState_Move_Trace* Create();
    virtual void Free() override;
};

NS_END