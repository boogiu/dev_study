#pragma once
#include "CMonsterState.h"
class CArchorAttackA :
    public CMonsterState
{
public:
    CArchorAttackA();
    ~CArchorAttackA();


    // CMonsterState을(를) 통해 상속됨
    void Enter(CMonster* monster) override;

    void Update(CMonster* monster) override;

    bool Exit(CMonster* monster) override;
private:
    bool m_bSigned;
    bool m_bFired;
    CBaseEffect* m_atkSign;
};

