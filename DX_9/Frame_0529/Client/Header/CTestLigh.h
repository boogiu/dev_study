#pragma once
#include "CGameObject.h"

class CTestLigh :
    public Engine::CGameObject
{
private:
    explicit CTestLigh();
    virtual ~CTestLigh();

public:
    static CTestLigh* Create();

public:
    HRESULT Ready_GameObject() override;
    void Update_GameObject(_float&dt) override;
    void LateUpdate_GameObject(_float&dt) override;

private:
    void Key_Check(float dt);

private:
    void Free() override;
};

