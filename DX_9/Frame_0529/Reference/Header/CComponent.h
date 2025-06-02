#pragma once
#include "CBase.h"
#include "CRenderMgr.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CComponent :
    public CBase
{
public:
    explicit CComponent() = default;
    virtual ~CComponent() =default;

public: 
    virtual HRESULT Ready_Component() = 0;
    virtual void Update_Component(float dt) = 0;
    virtual void LateUpdate_Component(float dt) = 0;
    virtual CComponent* Clone() const = 0;

public :
    virtual COM_TYPE Get_Type() = 0;
public:
    CGameObject* m_pOwner = nullptr;

};

template<typename T>
void RegisterOnSystem(T* component) {}

template<typename T>
void ReleaseOnSystem(T* component) {}

template<>
inline  void RegisterOnSystem<CRenderer>(CRenderer* renderer)
{
    CRenderMgr::GetInstance()->Add_Renderer(renderer);
}

template<>
inline void ReleaseOnSystem<CRenderer>(CRenderer* renderer)
{
    CRenderMgr::GetInstance()->Remove_Renderer(renderer);
}

END

