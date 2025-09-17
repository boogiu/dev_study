#pragma once
#include "Base.h"
#include "Transform.h"
#include "Builder.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGameObject abstract:
    public CBase
{
protected:
   CGameObject();
   CGameObject(const CGameObject& rhs);
    virtual ~CGameObject() DEFAULT;

public:
    template<typename T, typename ...Args>
    T* Add_Component(Args&& ...args);
    template<typename T>
    T* Get_Component();

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(INIT_DESC* pArg = nullptr);
    virtual void Engine_Update(_float dt);
    virtual void Priority_Update(_float dt) PURE;
    virtual void Update(_float dt) PURE;
    virtual void Late_Update(_float dt) PURE;

    virtual void Render_GUI();
    const string& Get_InstanceName() { return m_InstanceName; }
    const _uint Get_ObjectID() { return m_ObjectID; }
    _float4x4* Get_WorldMatrix();
    _float4 Get_Position();

protected:
    string m_InstanceName = {};
    _uint m_ObjectID = {};
    CTransform* m_pTransform = { nullptr };
    map<type_index,class CComponent*> m_Components;

public:
    virtual CGameObject* Clone(INIT_DESC* pArg = nullptr)PURE;
    virtual void Free() ;

    static _uint s_NextID;
};

NS_END

template<typename T, typename ...Args>
inline T* CGameObject::Add_Component(Args && ...args)
{
    T* comp = Get_Component<T>();
    if (comp)
        return comp;

    comp = T::Create(forward<Args>(args)...);
    comp->Set_Owner(this);

    if (comp)
        m_Components.insert({ type_index(typeid(T)), comp });

    return comp;
}

template<typename T>
inline T* CGameObject::Get_Component()
{
    auto iter = m_Components.find(type_index(typeid(T)));
    
    if (iter != m_Components.end())
        return static_cast<T*>(iter->second);

    return nullptr;
}
