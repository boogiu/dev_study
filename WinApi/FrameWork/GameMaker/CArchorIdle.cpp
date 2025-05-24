#include "pch.h"
#include "CArchorIdle.h"
#include "CMonster.h"

CArchorIdle::CArchorIdle()
{
    aniFrame.startPoint = { 3,2 };
    aniFrame.endPoint = { 2,3 };
    aniFrame.LoopPoint = { 3,2 };
    aniFrame.isLoop = true;
    aniFrame.frameSpeed = 0.2f;
    aniFrame.m_spSize = { 100,100 };
}

CArchorIdle::~CArchorIdle()
{
}

void CArchorIdle::Enter(CMonster* monster)
{
	monster->SetAnim();
    monster->Set_Velocity(0.f, 0.f);
}

void CArchorIdle::Update(CMonster* monster)
{
	if (monster->Get_Target()) {
        monster->ChangeState(MONSTER_MOVE);
	}
}

bool CArchorIdle::Exit(CMonster* monster)
{
	return false;
}
