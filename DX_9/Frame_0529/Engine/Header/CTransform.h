#pragma once
#include "CComponent.h"
BEGIN(Engine)

class ENGINE_DLL  CTransform :
    public CComponent
{
private:
    explicit CTransform();
    virtual ~CTransform();

public:
    static CTransform * Create();

public:
    HRESULT Ready_Component() override;
    void Update_Component(float dt) override;
    void LateUpdate_Component(float dt) override;
    void Render() override;

    CComponent* Clone() const override;

public:
    void Set_Parent(CTransform* transform);
    const _matrix& Get_WorldMatrix() const { return m_WorldMat; }

    static COM_TYPE Get_StaticType() { return COM_TYPE::TRANSFROM; }
    COM_TYPE Get_Type() { return Get_StaticType(); };

private:
    void Free() override;

private:
    _vec3 m_vPos;
    _vec3 m_vScale;
    _vec3 m_vRotate;
    _vec3 m_vPivot;
    _vec3 m_vOrbit;
    
    _matrix m_WorldMat;
    CTransform* m_pParent;
};

END

