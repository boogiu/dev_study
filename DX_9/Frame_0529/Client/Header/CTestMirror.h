#pragma once
#include "CGameObject.h"

namespace Engine {
    class CTransform;
    class CMeshRenderer;
    class CReflectRenderer;
}

class CTestMirror :
    public Engine::CGameObject
{
public:
    explicit CTestMirror();
    virtual ~CTestMirror();
public:
    static CTestMirror* Create();
public:
    HRESULT Ready_GameObject() override;
    void Update_GameObject(_float& dt) override;
    void LateUpdate_GameObject(_float& dt) override;
    void Set_Target(CGameObject* obj);
private:
    CTransform* m_pTransform;
    CMeshRenderer* m_pRenderer;
    CReflectRenderer* m_pMirror;

private:
    void Free() override;
};

