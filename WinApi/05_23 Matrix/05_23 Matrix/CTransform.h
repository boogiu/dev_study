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
    void Late_Update(float dt) override;
    static ComponentType StaticType() { return ComponentType::Transform; }
    ComponentType GetType() const override { return StaticType(); }

public:
    void Set_Scale(float _x, float _y, float _z);
    void Set_Position(float _x, float _y, float _z);
    void Set_Position(D3DXVECTOR3 _pos);
    void Set_Rotation(float _x, float _y, float _z);

    void Add_Position(float _x, float _y, float _z);
    void Add_RotateX(float x);
    void Add_RotateY(float y);
    void Add_RotateZ(float z);
    void Add_Position(D3DXVECTOR3 vDir);
    void Add_Rotate(D3DXVECTOR3 vRot);

    void Set_Pivot(float _x, float _y, float _z);
    void Set_Orbit(float _x, float _y, float _z);

    void Set_Pivot(D3DXVECTOR3 pivot);
    void Set_Orbit(D3DXVECTOR3 orbit);

    D3DXVECTOR3 Get_Scale();
    D3DXVECTOR3 Get_Position_Local();
    D3DXVECTOR3 Get_Position_World();
    D3DXVECTOR3 Get_Rotation();
    RECTANGLE Get_Rectangle();
    
    void Set_Parent(CTransform* parent);
    CTransform* Get_Parent();
    D3DXMATRIX Get_WorldMatrix();

private:
    RECTANGLE m_tRect;
    D3DXMATRIX m_WorldMat;
    D3DXVECTOR3 m_vPivot;
    D3DXVECTOR3 m_vOrbit;
    CTransform* m_pParent;
   };

