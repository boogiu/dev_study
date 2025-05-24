#pragma once
#include "CStage.h"
class CFirstStage :
    public CStage
{
public:
    CFirstStage();
    ~CFirstStage()override;

public:
    // CStage을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:
    bool m_bStageEnd;
    bool readyToNext;
    bool m_bisStart;
};
