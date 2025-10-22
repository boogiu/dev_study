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
    virtual void OnEnter() {}
    virtual void OnUpdate(_float dt) {}
    virtual void OnExit() {}
    virtual CState* HandleTransition() { return nullptr; }

public:
    void SetParent(CState* parent) { m_pParent = parent; }
    CState* GetParent() const { return m_pParent; }

    const string& GetName() const { return m_StateName; }
    void SetName(const string& name) { m_StateName = name; }
public:
    virtual void Render_State();
protected:
    CState* m_pParent = { nullptr };
    string m_StateName = {};
    _bool m_bEnter = false;
public:
    virtual void Free();
};

NS_END