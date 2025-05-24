#pragma once
#include "CMonsterState.h"
class CSoldierHit :
    public CMonsterState
{
public:
    CSoldierHit();
    ~CSoldierHit();

public:
    // CMonsterState을(를) 통해 상속됨
    void Enter(CMonster* monster) override;
    void Update(CMonster* monster) override;
    bool Exit(CMonster* monster) override;

private:
    float m_elapsedTime;
};

