#pragma once
#include "CMonsterState.h"
class CGiantAttackA :
    public CMonsterState
{
public:
    CGiantAttackA();
    ~CGiantAttackA();

public:
    // CMonsterState을(를) 통해 상속됨
    void Enter(CMonster* monster) override;
    void Update(CMonster* monster) override;
    bool Exit(CMonster* monster) override;

private:
    float m_elapsedTime;
    CAttackBox* m_pAtkBox;
    int m_iCheckDir;
    bool m_bDirCheck;
    CBaseEffect* m_pTackle_Effect;
};

