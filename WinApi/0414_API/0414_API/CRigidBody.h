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
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    VECTOR2& GetVelocity() { return m_vVelocity; }
    void SetVelocity(VECTOR2 _velocity) { m_vVelocity = _velocity; }
    void ImPulse(VECTOR2 _velocity);

private:
    VECTOR2 m_vVelocity;
};

