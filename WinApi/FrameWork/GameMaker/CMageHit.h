#pragma once
#include "CMonsterState.h"
class CMageHit :
    public CMonsterState
{
public:
    CMageHit();
    ~CMageHit();
public:
    void Enter(CMonster* monster) override;
    void Update(CMonster* monster) override;
    bool Exit(CMonster* monster) override;
private:
    float m_ElapsedTime;
};

