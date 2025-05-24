#pragma once
#include "CStage.h"
class CBossBackGround;
class CBoss;
class CBossFrame;

class CBossStage :
    public CStage
{
public:
    CBossStage();
    ~CBossStage()override;

public:
    // CStage을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:
    void StartIntro();
    void Phase1Pattern();
    void Phase2Pattern();
    void Check_Boss_Hp();
    void CheckAndEnterWaitState(CBoss* boss);
    bool HandleBossDeathTransition(CBoss* deadBoss, CBoss* survivingBoss);
    void Stage_End();
private:
    POINT m_WaitPtA;
    POINT m_WaitPtB;
    CBossBackGround* m_pBackBoss;
    CBoss* m_pBoss1;
    CBoss* m_pBoss2;
    CBoss* m_pBoss3;
    CBossFrame* m_pUI;

    bool m_bIntroEnd;
    bool m_bStopPattern;
    bool m_b2phase;
    bool m_bBoss3DeadHandled;
    bool m_bBossDead;
    bool m_bAppearBox;
    bool m_bbackRun;

    float m_IntroTime;
    float m_patternTime;
    float m_Bgm;
    float m_DeadTime;
};

