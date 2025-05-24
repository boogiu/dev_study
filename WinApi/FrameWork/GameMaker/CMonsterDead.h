#pragma once
#include "CMonsterState.h"
class CMonsterDead :
    public CMonsterState
{
public:
    CMonsterDead();
    ~CMonsterDead();
public:

    // CMonsterState을(를) 통해 상속됨
    void Enter(CMonster* monster) override;
    void Update(CMonster* monster) override;
    bool Exit(CMonster* monster) override;
};

