#pragma once
#include "CGameObject.h"

namespace Engine {
    class CCamera;
    class CTransform;
}

class CTestCam :
    public Engine::CGameObject
{
private:
    explicit CTestCam();
    virtual ~CTestCam();

public:
    static CTestCam* Create();
public:
    HRESULT Ready_GameObject() override;
    void Update_GameObject(float dt) override;
    void LateUpdate_GameObject(float dt) override;
public:
    Engine::CCamera* Get_Camera() { return m_pCamera; }
private:
    void Key_Check(float dt);

private:
    Engine::CTransform* m_pTransform;
    Engine::CCamera* m_pCamera;
    Engine::CGameObject* m_pTarget;
private:
    virtual void Free();
};

