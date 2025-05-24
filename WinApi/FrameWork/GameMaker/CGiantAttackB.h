#pragma once
#include "CMonsterState.h"
class CGiantAttackB :
    public CMonsterState
{
public:
    CGiantAttackB();
    ~CGiantAttackB();
public:
    // CMonsterState을(를) 통해 상속됨
    void Enter(CMonster* monster) override;
    void Update(CMonster* monster) override;
    bool Exit(CMonster* monster) override;

private:;
       void Create_Wave();
private:
    bool m_bWaveStart;
    bool m_bCheckDir;
    int m_iCheckedDir;
    int m_WaveCnt;
    float m_WaveDelay;
    float m_ElapsedTime;
    ANI_FRAME effectFrame;
    INFO m_tAtkInfo;
    CAttackBox* m_atkArr[5];
};

