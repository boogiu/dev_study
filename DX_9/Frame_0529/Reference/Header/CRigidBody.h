#pragma once
#include "CComponent.h"

BEGIN(Engine)
class CTransform;

class ENGINE_DLL CRigidBody :
    public CComponent
{
public:
    enum class  Rigid_State { KINEMATIC, STATIC, DYNAMIC };
    static CRigidBody* Create(Rigid_State state);

public:
    HRESULT Ready_Component() override;
    void Update_Component(float& dt) override;
    void LateUpdate_Component(float& dt) override;
    CComponent* Clone() const override;

public: 
    void Add_Impulse(_vec3 velocity);
    void Set_Impulse(_vec3 velocity) { m_vVelocity = velocity; }
    void Set_State(Rigid_State state) { m_eState = state;}
    void Set_Transform();
    void Set_Friction(float friction) { m_fFriction = friction; }

private:
    bool m_bGrounded;
    float m_fFriction;

    Rigid_State m_eState;
    CTransform *  m_pTransform;
    _vec3 m_vVelocity;
private:
    void Free() override;
};

END