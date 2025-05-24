#pragma once
#include "CMonsterState.h"
class CMageCore;

class CMageAttackA :
    public CMonsterState
{
public:
    CMageAttackA();
    ~CMageAttackA();
public:
    void Enter(CMonster* monster) override;
    void Update(CMonster* monster) override;
    bool Exit(CMonster* monster) override;

private:
    bool m_bFired;
    CMageCore* m_pCore;
};

