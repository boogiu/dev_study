#pragma once
#include "Base.h"
#include "Player.h"

NS_BEGIN(Client)

class CStateMachine abstract:
    public CBase
{
protected:
    CStateMachine();
    virtual ~CStateMachine() DEFAULT;

public:
    virtual void Update(_float dt) PURE;
    virtual void Request_ChangeState(STATE_LAYER eLayer, const string& NextState) PURE;

public:
    virtual void OnCollisionEnter(COLLISION_CONTEXT context);
    virtual void OnCollisionStay(COLLISION_CONTEXT context);
    virtual void OnCollisionExit(COLLISION_CONTEXT context);

public:
    class CLayerState* Get_State(STATE_LAYER eLayer) { return m_LayerStates[eLayer]; }
protected:
    map<STATE_LAYER, class CLayerState*> m_LayerStates;
public:
    virtual void Free();
};

NS_END