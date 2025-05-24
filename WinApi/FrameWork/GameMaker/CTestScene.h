#pragma once
#include "CScene.h"
class CStage;
class CObject;

class CTestScene :
    public CScene
{
public:
    CTestScene();
    ~CTestScene() override;
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:
    CStage* m_NowStage;

    // CScene을(를) 통해 상속됨
    void CloseScene() override;
};

