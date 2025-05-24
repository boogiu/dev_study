#include "pch.h"
#include "CGiantIdle.h"
#include "CMonster.h"
CGiantIdle::CGiantIdle()
{
    aniFrame.startPoint = {1,2 };
    aniFrame.endPoint = { 0,3 };
    aniFrame.isLoop = true;
    aniFrame.LoopPoint = { 1,2 };
    aniFrame.frameSpeed = 0.2f;
    aniFrame.m_spSize = { 200,150 };
}

CGiantIdle::~CGiantIdle()
{
}

void CGiantIdle::Enter(CMonster* monster)
{
    monster->SetAnim();
}

void CGiantIdle::Update(CMonster* monster)
{
    if (monster->Get_Target() && monster->Can_Attack()) {
        monster->ChangeState(MONSTER_MOVE);
    }
}

bool CGiantIdle::Exit(CMonster* monster)
{
    return false;
}
