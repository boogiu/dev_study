#pragma once
#include "CScene.h"


class CStageScene :
    public CScene
{
public:
    CStageScene();
    virtual ~CStageScene()override;
public:
    // CScene을(를) 통해 상속됨
   virtual void Initialize() override;
   virtual void Update() override;
   virtual void Render(HDC _hDC) override;
   virtual void Release() override;

};

