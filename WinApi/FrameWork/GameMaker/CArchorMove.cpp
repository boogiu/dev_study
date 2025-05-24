#include "pch.h"
#include "CArchorMove.h"
#include "CMonster.h"
CArchorMove::CArchorMove()
{
	aniFrame.startPoint = { 3,3 };
	aniFrame.endPoint = { 0,5 };
	aniFrame.LoopPoint = { 3,3 };
	aniFrame.isLoop = true;
	aniFrame.frameSpeed = 0.2f;
	aniFrame.m_spSize = { 100,100 };
}

CArchorMove::~CArchorMove()
{
}

void CArchorMove::Enter(CMonster* monster)
{
	monster->SetAnim();
}

void CArchorMove::Update(CMonster* monster)
{
	if (monster->Get_Target()) {
		float distance = fabs(monster->Get_Info().fX - monster->Get_Target()->Get_Info().fX);
		if (distance < 100 &&monster->Can_Attack()) {
			monster->ChangeState(MONSTER_ATTACKB);
		}
		else if (distance > 250 && monster->Can_Attack()) {
			monster->ChangeState(MONSTER_ATTACKA);
		}
		else if (distance > 80) {
			monster->Add_Velocity(2 * monster->Get_Dir(), 0.f);
		}
	}
	else {
		monster->ChangeState(MONSTER_IDLE);
	}
}

bool CArchorMove::Exit(CMonster* monster)
{
	return false;
}
