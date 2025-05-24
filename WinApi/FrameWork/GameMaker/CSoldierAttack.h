#pragma once
#include "CMonsterState.h"
class CSoldierAttack :
    public CMonsterState
{
public:
    CSoldierAttack();
    ~CSoldierAttack();
public:


    // CMonsterState을(를) 통해 상속됨
    void Enter(CMonster* monster) override;

    void Update(CMonster* monster) override;

    bool Exit(CMonster* monster) override;

private:
    bool m_bAttack;
    CAttackBox* m_pAtkBox;
};

