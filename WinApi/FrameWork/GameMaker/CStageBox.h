#pragma once
#include "CObject.h"

class CAnim;

class CStageBox :
    public CObject
{
public:
    CStageBox();
    ~CStageBox();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:
    bool m_bBoxOpen;

    CAnim* m_pAnim;
    ANI_FRAME m_tAnim;
};

