#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
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
 
private:
    _float fLerpPercent = 10.f;
    _float fMouseSensitive = 0.1f;
    _float2 m_vRotationVelocity = { 0.f, 0.f };
public:
    static CFree_Camera* Create();
    virtual void Free() override;
    CGameObject* Clone(INIT_DESC* pArg) override;
};

NS_END