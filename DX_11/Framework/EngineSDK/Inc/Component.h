#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL CComponent abstract:
    public CBase
{
protected:
    CComponent();
    CComponent(const CComponent& rhs);

    virtual ~CComponent() override DEFAULT;
public:
    virtual void Priority_Update(_float dt) PURE;
    virtual void Update(_float dt) PURE;
    virtual void Late_Update(_float dt) PURE;
public:
    void Set_Owner(class CGameObject* owner) { m_pOwner = owner; }
protected:
    class CGameObject* m_pOwner;
};

NS_END