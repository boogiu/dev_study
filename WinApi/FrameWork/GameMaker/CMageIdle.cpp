#include "pch.h"
#include "CMageIdle.h"
#include "CMonster.h"
#include "CTimeMgr.h"

CMageIdle::CMageIdle()
{
    aniFrame.startPoint = { 2,1 };
    aniFrame.endPoint = {2 ,2 };
    aniFrame.LoopPoint = { 2,1 };
    aniFrame.isLoop = true;
    aniFrame.frameSpeed = 0.2f;
    aniFrame.m_spSize = { 100,100 };
}

CMageIdle::~CMageIdle()
{
}

void CMageIdle::Enter(CMonster* monster)
{
    monster->SetAnim();
    m_ElapsedTime = 0;
}

void CMageIdle::Update(CMonster* monster)
{
    m_ElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
    if (m_ElapsedTime > 2.f) {
        monster->ChangeState(MONSTER_MOVE);
    }
}

bool CMageIdle::Exit(CMonster* monster)
{
    return false;
}
