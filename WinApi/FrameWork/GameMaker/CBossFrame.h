#pragma once
#include "CBase_UI.h"
class CIcon;
class CBossFrame :
    public CBase_UI
{
public:
    CBossFrame();
    ~CBossFrame() override;
public:

    // CBase_UI을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void CheckBossAHP(float hpRatio);
    void CheckBossBHP(float hpRatio);
    void CheckBossCHP(float hpRatio);
    void ChangePhase();
private:
    unordered_map < const TCHAR*, CIcon* > m_IconContainer;
    bool m_bPahse2;
    float Phase1Length;
    float Phase2Length;
};

