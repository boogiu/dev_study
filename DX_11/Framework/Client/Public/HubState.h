#pragma once
#include "State.h"
NS_BEGIN(Client)
class CHubState :
    public CState
{
protected:
    CHubState();
    virtual ~CHubState() DEFAULT;

public:
    virtual void DecideSubState(_float dt) PURE;
    void Execute(CState* rootState) { m_pCurrentState = rootState; }
    void Change_State(const string name);
    
protected:
    CState* Get_State(const string state);
public:
    virtual void Free() override;

protected:
     CState* m_pCurrentState = { nullptr };
    unordered_map<string, class CState*> m_SubStates;
};
NS_END
