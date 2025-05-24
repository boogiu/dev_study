#include "pch.h"
#include "CMonsterDead.h"
#include "CMonster.h"
#include "CRader.h"
#include "CResourceMgr.h"
#include "CObjMgr.h"
#include "CParticle.h"
#include "CFunction.h"
CMonsterDead::CMonsterDead()
{
	aniFrame.startPoint = { 0,0 };
	aniFrame.endPoint = { 1,2 };
	aniFrame.isLoop = false;
	aniFrame.frameSpeed = 0.1f;
	aniFrame.m_spSize = { 150,150 };
}

CMonsterDead::~CMonsterDead()
{
}

void CMonsterDead::Enter(CMonster* monster)
{
	monster->Get_Info().Set_Size(250.f, 250.f);
	monster->ChangeAnimPath(L"Enemy_Dead");
	CResourceMgr::Get_Instance()->PlaySoundW(L"Enemy_Dead", 0.24f);

	for (int i = 0; i < 3; ++i) {
		CParticle* parts = static_cast<CParticle*>(CObjMgr::Get_Instance()->ReQuestObject<CParticle>(PARTICLE));
		int j = CFunction::GetInt(1, 9);
		parts->Set_Name(monster->Get_Name(), j);
		parts->Set_Pos(monster->Get_Info().fX, monster->Get_Info().fY);
		parts->Initialize();
	}
}

void CMonsterDead::Update(CMonster* monster)
{
	if (monster->Get_Anim()->isAnimEnd()) {
		monster->Get_Rader()->Set_Active(false);
		monster->Set_Active(false);
	}
}

bool CMonsterDead::Exit(CMonster* monster)
{
	
	
	monster->Set_Active(false);
	return false;
}
