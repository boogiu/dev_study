#pragma once
#include "CGameObject.h"

namespace Engine{
    class CTransform;
    class CTerrainRenderer;
}

class CTestTerrain :
    public Engine::CGameObject
{
public:
    explicit CTestTerrain();
    virtual ~CTestTerrain();
public:
    static CTestTerrain* Create();
public:
    HRESULT Ready_GameObject() override;
    void Update_GameObject(_float&dt) override;
    void LateUpdate_GameObject(_float&dt) override;

private:
    CTransform* m_pTransform;
    CTerrainRenderer* m_pRenderer;
private:
    void Free() override;
};

