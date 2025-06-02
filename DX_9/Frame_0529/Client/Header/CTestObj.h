#pragma once
#include "CGameObject.h"
#include "CComponent.h"
#include "CMesh.h"
#include "CTransform.h"
#include "CRenderer.h"

class CTestObj :
    public Engine::CGameObject
{

private:
    explicit CTestObj();
    virtual ~CTestObj();
public:
    static CTestObj* Create();

public:
    HRESULT Ready_GameObject() override;
    void Update_GameObject(float dt) override;
    void LateUpdate_GameObject(float dt) override;
private:
    void Key_Check(float dt);
private:
    void Free() override;

private:
    CTransform* m_pTransform;
    CMesh* m_pMesh;
    CRenderer* m_pRenderer;
};

