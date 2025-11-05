#pragma once
#include "Base.h"

NS_BEGIN(Client)

class CState :
    public CBase
{
protected:
    CState();
    virtual ~CState() DEFAULT;

public:
    virtual HRESULT OnEnter() { return S_OK; }
    virtual void OnUpdate(_float dt) {}
    virtual HRESULT OnExit() { return S_OK; }
    virtual CState* HandleTransition() { return nullptr; }

public:
    virtual void OnCollisionEnter(COLLISION_CONTEXT context);
    virtual void OnCollisionStay(COLLISION_CONTEXT context);
    virtual void OnCollisionExit(COLLISION_CONTEXT context);

public:
    virtual void Set_Owner(class CGameObject* pOwner) {};
    const string& GetName() const { return m_StateName; }
    void SetName(const string& name) { m_StateName = name; }
    void SetLayer(class CLayerState* layer) { m_pLayer = layer; }
    void Set_Machine(class CStateMachine* machine) { m_pStateMachine = machine; }

public:
    virtual void Render_State();
    virtual _uint Get_InputMask() const { return 0xFFFFFFFF; }

protected:
    class CStateMachine* m_pStateMachine = { nullptr };
    class CLayerState* m_pLayer = { nullptr };

    string m_StateName = {};
    _bool m_bEnter = false;

public:
    virtual void Free();
};

NS_END