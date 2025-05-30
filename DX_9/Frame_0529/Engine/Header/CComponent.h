#pragma once
#include "CBase.h"
BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CComponent :
    public CBase
{
protected:
    explicit CComponent() = default;
    virtual ~CComponent() =default;
public: 
    virtual HRESULT Ready_Component() = 0;
    virtual void Update_Component(float dt) = 0;
    virtual void LateUpdate_Component(float dt) = 0;
    virtual void Render() = 0;
    virtual CComponent* Clone() const = 0;
public :
    virtual COM_TYPE Get_Type() = 0;
public:
    CGameObject* m_pOwner;
};

template<typename T>
void RegisterOnSystem(T* component) {}

template<typename T>
void ReleaseOnSystem(T* component) {}

// 특수화는 선언만 남김

//template<>
//void RegisterOnSystem<CRenderer>(CRenderer* renderer);

//template<>
//void ReleaseOnSystem<CRenderer>(CRenderer* renderer);


END