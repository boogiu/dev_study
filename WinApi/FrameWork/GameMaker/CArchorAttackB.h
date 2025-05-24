#pragma once
#include "CMonsterState.h"
class CArchorAttackB :
    public CMonsterState
{
public:
    CArchorAttackB();
    ~CArchorAttackB();
public:
    void Enter(CMonster* monster) override;
    void Update(CMonster* monster) override;
    bool Exit(CMonster* monster) override;

private :
    float m_dir;
    bool m_bFired;
    bool m_bGround;
    int JumpPow;
    int BackStepPow;
};

