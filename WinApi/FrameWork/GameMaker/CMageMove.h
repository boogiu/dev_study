#pragma once
#include "CMonsterState.h"
class CMageMove :
    public CMonsterState
{
public:
    CMageMove();
    ~CMageMove();
public:
    void Enter(CMonster* monster) override;
    void Update(CMonster* monster) override;
    bool Exit(CMonster* monster) override;

private:
    bool m_bTeleporting;
    bool m_bTeleportEnd;
    bool m_bFindTile;
    bool m_bMoveIn;
    bool m_bMoveOut;
    float m_elapsedTime;
};

