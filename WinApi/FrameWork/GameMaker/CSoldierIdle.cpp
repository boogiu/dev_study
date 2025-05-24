#include "pch.h"
#include "CSoldierIdle.h"
#include "CMonster.h"

CSoldierIdle::CSoldierIdle()
{
    aniFrame.startPoint = {3,1};
    aniFrame.endPoint = { 2,2 };
    aniFrame.LoopPoint = {3,1};
    aniFrame.isLoop = true;
    aniFrame.frameSpeed = 0.2f;
    aniFrame.m_spSize = {100,100};
}

CSoldierIdle::~CSoldierIdle()
{
}


void CSoldierIdle::Enter(CMonster* monster)
{
    monster->SetAnim();
}

void CSoldierIdle::Update(CMonster* monster)
{
}

bool CSoldierIdle::Exit(CMonster* monster)
{
    return false;
}
