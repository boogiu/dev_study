#pragma once
#include "CScene.h"

namespace Engine {
    class CGameObject;
}
class CTestCam;
class CTestTerrain;
class CTestMirror;
class CTestScene :

    public Engine::CScene
{
private:
    explicit CTestScene();
    virtual ~CTestScene();

public:
    static CTestScene* Create();

public:
    void Update_Scene(_float& dt) override;
    void LateUpdate_Scene(_float& dt) override;
    void Render_Scene(LPDIRECT3DDEVICE9 pDevice) override;

public:
    HRESULT Ready_Scene() override;
    HRESULT Enter_Scene() override;
    HRESULT Exit_Scene() override;

public:
    CGameObject* m_pCam;

private:
    void Free() override;
};

