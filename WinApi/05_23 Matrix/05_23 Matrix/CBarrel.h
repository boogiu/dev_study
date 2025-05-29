#pragma once
#include "CObject.h"
class CBarrel :
    public CObject
{
public:
    CBarrel();
    virtual  ~CBarrel() override;
public:
    void Initialize() override;
    void Update(float dt) override;
    void Late_Update(float dt) override;
    void Release() override;

    static ObjectType StaticType() { return ObjectType::WEAPON; }
    ObjectType GetType() override { return StaticType(); };

private:
    float m_fAngle;
};

