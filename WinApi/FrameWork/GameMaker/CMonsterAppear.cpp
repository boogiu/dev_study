#include "pch.h"
#include "CMonsterAppear.h"
#include "CMonster.h"

CMonsterAppear::CMonsterAppear()
{
	aniFrame.startPoint = { 0,0 };
	aniFrame.endPoint = { 2,3 };
	aniFrame.isLoop = false;
	aniFrame.frameSpeed = 0.05f;
	aniFrame.m_spSize = { 150,150 };
}

CMonsterAppear::~CMonsterAppear()
{
}

void CMonsterAppear::Enter(CMonster* monster)
{
	//monster->Get_Info().Set_Size(150.f, 150.f);
	monster->ChangeAnimPath(L"Enemy_Appearance");
}

void CMonsterAppear::Update(CMonster* monster)
{
	if (monster->Get_Anim()->isAnimEnd()) {
		monster->ChangeState(MONSTER_IDLE);
	}
}

bool CMonsterAppear::Exit(CMonster* monster)
{
    return false;
}
