#pragma once
#include "GameObject.h"

NS_BEGIN(Loader)
class CFree_Camera :
    public CGameObject
{
private:
    CFree_Camera();
    CFree_Camera(const CFree_Camera& rhs);
    virtual ~CFree_Camera()DEFAULT;
public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
public:
    virtual void Render_GUI() override;
private:
    _float m_fDistance = {};
    _float m_fYaw = {};
    _float m_fPitch = {45.f};
    _float m_fSpeed = {40.f};
    _float3 m_vPivot = {0,0,0};
public:
    static CFree_Camera* Create();
    virtual void Free() override;
    CGameObject* Clone(INIT_DESC* pArg) override;
};

NS_END