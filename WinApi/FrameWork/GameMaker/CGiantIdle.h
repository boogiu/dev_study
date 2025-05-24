#pragma once
#include "CMonsterState.h"
class CGiantIdle :
    public CMonsterState
{
public:
    CGiantIdle();
    ~CGiantIdle();

public:
    // CMonsterState을(를) 통해 상속됨
    void Enter(CMonster* monster) override;
    void Update(CMonster* monster) override;
    bool Exit(CMonster* monster) override;

};

