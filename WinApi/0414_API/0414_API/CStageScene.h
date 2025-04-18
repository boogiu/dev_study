#pragma once
#include "CScene.h"
class CStageScene :
    public CScene
{
public:
    CStageScene();
    ~CStageScene();

public:
    // CScene을(를) 통해 상속됨
   virtual void Initialize() override;
   virtual void Update() override;
   virtual void Late_Update() override;
   virtual void Render(HDC _hDC) override;
   virtual void Release() override;
};

