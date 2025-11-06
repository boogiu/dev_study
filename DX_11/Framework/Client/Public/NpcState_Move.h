#pragma once
#include "NpcState.h"
NS_BEGIN(Client)
class CNpcState_Move :
    public CNpcState
{
private:
    CNpcState_Move();
    virtual ~CNpcState_Move() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

public:
    static CNpcState_Move* Create();
    virtual void Free();
};

NS_END