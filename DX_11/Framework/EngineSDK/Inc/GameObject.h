#pragma once
#include "Base.h"
#include "Transform.h"
#include "Builder.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGameObject abstract:
    public CBase
{
public:
    typedef struct tagGameObjectDesc : public INIT_DESC {
        string InstanceName = "";
        unordered_map<type_index, INIT_DESC*> m_CompDesc;

        virtual ~tagGameObjectDesc() DEFAULT;
    }GAMEOBJECT_DESC;

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
    virtual void Priority_Update(_float dt) PURE;
    virtual void Update(_float dt) PURE;
    virtual void Late_Update(_float dt) PURE;

protected:
    string m_InstanceName;
    CTransform* m_pTransform = { nullptr };
    unordered_map<type_index,class CComponent*> m_Components;

public:
    virtual CGameObject* Clone(INIT_DESC* pArg = nullptr)PURE;
    virtual void Free() ;
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
