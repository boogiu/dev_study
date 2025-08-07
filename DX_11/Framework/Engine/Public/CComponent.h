#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL CComponent abstract:
    public CBase
{
protected:
    CComponent();
    CComponent(CComponent& rhs);
    virtual ~CComponent() override DEFAULT;
    void Set_Owner(CGameObject* owner) { m_pOwner = owner; }

protected:
    class CGameObject* m_pOwner;
};

NS_END