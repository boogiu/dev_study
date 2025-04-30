#pragma once
#include "CStage.h"
class CTestStage :
    public CStage
{
public:
    CTestStage();
    ~CTestStage()override;

public:
    // CStage을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

};

