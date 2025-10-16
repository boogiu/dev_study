#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CTarget_Camera :
    public CGameObject
{
public:
    typedef struct tagTargetCamDesc : GAMEOBJECT_DESC {
        CGameObject* pTarget = { nullptr };
    }TARGET_CAM_DESC;
private:
    CTarget_Camera();
    CTarget_Camera(const CTarget_Camera& rhs);
    virtual ~CTarget_Camera()DEFAULT;
public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

private:
    CGameObject* m_pTarget = { nullptr };

public:
    static CTarget_Camera* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free() override;
};

NS_END