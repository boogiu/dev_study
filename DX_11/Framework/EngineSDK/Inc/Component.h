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
    virtual HRESULT Initialize_Prototype() PURE;
    virtual HRESULT Initialize(INIT_DESC* pArg = nullptr)PURE;
public:
    void Set_Owner(class CGameObject* owner) { m_pOwner = owner; }
protected:
    class CGameObject* m_pOwner;

public:
    virtual CComponent* Clone() PURE;
    virtual void Free();
};

NS_END