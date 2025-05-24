#pragma once
#include "CComponent.h"
class CTransform :
    public CComponent
{
public:
    CTransform();
    ~CTransform();

public:
    void Initialize() override;
    void Update(float dt) override;
    static ComponentType StaticType() { return ComponentType::Transform; }
    ComponentType GetType() const override { return StaticType(); }

public:
    void Set_Scale(float _x, float _y, float _z);
    void Set_Position(float _x, float _y, float _z);
    void Set_Rotation(float _x, float _y, float _z);

    D3DXVECTOR3 Get_Scale();
    D3DXVECTOR3 Get_Position();
    D3DXVECTOR3 Get_Rotation();

private:
    D3DXVECTOR3 m_vPos;
    D3DXVECTOR3 m_vScale;
    D3DXVECTOR3 m_vRotate;
};

