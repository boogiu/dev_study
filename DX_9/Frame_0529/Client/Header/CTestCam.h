#pragma once
#include "CGameObject.h"

namespace Engine {
    class CCamera;
    class CTransform;
    class CLight;
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
    void Update_GameObject(_float&dt) override;
    void LateUpdate_GameObject(_float&dt) override;
public:
    Engine::CCamera* Get_Camera() { return m_pCamera; }
private:
    void Key_Check(_float&dt);

private:
    CTransform* m_pTransform;
    CCamera* m_pCamera;
    CLight* m_pLight;

private:
    virtual void Free();
};

