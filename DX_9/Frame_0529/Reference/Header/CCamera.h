#pragma once
#include "CComponent.h"
BEGIN(Engine)
class CTransform;

class ENGINE_DLL CCamera :
    public CComponent
{
private:
    explicit CCamera();
    virtual ~CCamera() override;

public:
    static CCamera* Create();
    HRESULT Ready_Component() override;
    void Update_Component(float& dt) override;
    void LateUpdate_Component(float& dt) override;
    CComponent* Clone() const override;

public:
    static COM_TYPE Get_StaticType() { return COM_TYPE::CAMERA; }
    COM_TYPE Get_Type() override { return Get_StaticType(); };

    void Set_View(_vec3 _vEye, _vec3 _vAt, _vec3 _vUP);
    void Set_Proj(float _FOV,float _aspect, float _near, float _far);

    _matrix& Get_ViewMatrix() { return m_matView; }
    _matrix& Get_ProjMatrix() { return m_matProj; }

    void Add_Yaw(float angle);
    void Add_Pitch(float angle);
    void Add_Roll(float angle);

    _vec3 Get_Dir() { return m_vLookDir; };
private:
    CTransform* m_pTransform;
    _vec3 m_vEye;
    _vec3 m_vLookDir;
    _vec3 m_vUp;
    _matrix m_matView;

    float m_fFOV;
    float m_fAspect;
    float m_fNear;
    float m_fFar;

    _matrix m_matProj;

private:
    void Free() override;
};

END
