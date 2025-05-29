#pragma once
#include "CComponent.h"
class CRigidBody :
    public CComponent
{
public:
    CRigidBody();
    ~CRigidBody();
public:
    void Initialize() override;
    void Update(float dt) override;
    void Late_Update(float dt) override;
    static ComponentType StaticType() { return ComponentType::RigidBody; }
    ComponentType GetType() const override { return StaticType(); }
public:
    void Impulse(float _x, float _y, float _z);
    void ImpulseRotate(float _x, float _y, float _z);
    void Set_Friction(bool set, float friction = 0.95f);
private:
    bool m_bFriction;
    float m_fFriction;

    D3DXVECTOR3 m_vVelocity;
    D3DXVECTOR3 m_vAngular;
};

