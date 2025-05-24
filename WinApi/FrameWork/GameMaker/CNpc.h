#pragma once
#include "CObject.h"
class CBase_UI;

class CNpc :
    public CObject
{
public:
    CNpc();
    ~CNpc();

    // CObject을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType) override;

private:
    void SetDialougue();
private:
    HDC whiteDC = nullptr;
    HBITMAP hWhiteBitmap = nullptr;
    bool m_bFlashWhite = false;
    bool m_bFree;
    bool m_bTalking;
    float m_fFlashTime = 0.f;
    int AtkCount;
    CBase_UI* m_pDialogue;
};

