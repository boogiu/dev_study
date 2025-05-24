#pragma once
#include "CMonsterState.h"
class CArchorHit :
    public CMonsterState
{
public:
    CArchorHit();
    ~CArchorHit();
public:


    // CMonsterState을(를) 통해 상속됨
    void Enter(CMonster* monster) override;

    void Update(CMonster* monster) override;

    bool Exit(CMonster* monster) override;

private:
    float m_ElapsedTime;
};

