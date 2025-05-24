#pragma once
#include "CMonsterState.h"
class CMageIdle :
    public CMonsterState
{
public:
    CMageIdle();
    ~CMageIdle();
public:
    void Enter(CMonster* monster) override;
    void Update(CMonster* monster) override;
    bool Exit(CMonster* monster) override;

private:
    float m_ElapsedTime;
};

