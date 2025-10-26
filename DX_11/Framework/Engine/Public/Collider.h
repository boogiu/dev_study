#pragma once
#include "Component.h"
NS_BEGIN(Engine)
class ENGINE_DLL CCollider :
    public CComponent
{
protected:
    CCollider();
    CCollider(const CCollider& rhs);
    virtual ~CCollider() DEFAULT;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(COMPONENT_DESC* pArg) override;
    virtual void Update() PURE;
    virtual void Late_Update() PURE;

public:
   virtual _bool Intersect(CCollider* pOther) PURE;
   virtual _bool Has_Desc() PURE;
   virtual void Make_MinMaxCollider(MINMAX_BOX minMax)PURE;
   virtual void Set_ColliderActive(_bool Active);

#ifdef _DEBUG
    virtual void Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) PURE;
#endif
public:
    virtual COLLIDER_TYPE Get_ColliderType() PURE;

public:
    void Render_GUI();
protected:
    _int m_SystemIndex = { -1 };
    unordered_set<CCollider*> m_prevCollider = {}; //이전 프레임에 부딪힘
    unordered_set<CCollider*> m_CurrentCollider = {}; //현재 프레임에 부딪힘.

public:
    virtual void Free() override;
};

NS_END
