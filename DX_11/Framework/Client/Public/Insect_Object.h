#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CInsect_Object :
    public CGameObject
{
protected:
    CInsect_Object();
    CInsect_Object(const CInsect_Object& rhs);
    virtual ~CInsect_Object() DEFAULT;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(INIT_DESC* pArg) override;
    virtual void Priority_Update(_float dt) override;
    virtual void Update(_float dt) override;
    virtual void Late_Update(_float dt) override;

public:
    virtual virtual void Render_GUI()override;

public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context)override;
    void OnCollisionExit(COLLISION_CONTEXT context)override;

private:
    void MoveFly(_float dt);

private:
    _float m_fLifeTime = {};
    _float4 m_vPivotPos = {};

public:
    static CInsect_Object* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END