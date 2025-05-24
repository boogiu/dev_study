#include "pch.h"
#include "CArchorAttackB.h"
#include "CMonster.h"
#include "CObjMgr.h"
#include "CArrow.h"
#include "CResourceMgr.h"
CArchorAttackB::CArchorAttackB() :m_dir(0.f), JumpPow(0), BackStepPow(0), m_bFired(false), m_bGround(false)
{

	aniFrame.startPoint = { 1,1 };
	aniFrame.endPoint = { 0,2 };
	aniFrame.isLoop = false;
	aniFrame.LoopPoint = { 0,0 };
	aniFrame.frameSpeed = 0.1f;
	aniFrame.m_spSize = { 100,100 };
}

CArchorAttackB::~CArchorAttackB()
{
}

void CArchorAttackB::Enter(CMonster* monster)
{
	monster->SetAnim();
	CObject* target = monster->Get_Target();
	float fDistance = monster->Get_Info().fX - target->Get_Info().fX;
	monster->Set_Velocity(0.f, 0.f);
	if (fDistance <= 0) {
		m_dir = -1; //¿ÞÂÊÀ¸·Î ¶Ù±â
	}
	else {
		m_dir = 1; //¿À¸¥ ÂÊÀ¸·Î ¶Ù±â
	}
	BackStepPow = 120;
	JumpPow = 500;
	m_bFired = false;
	m_bGround = false;
	monster->Get_Anim()->Set_BreakPoint({ 1,1 });
	monster->Add_Velocity(m_dir * BackStepPow, -1 * JumpPow);
}

void CArchorAttackB::Update(CMonster* monster)
{
	if (!m_bGround && !monster->isOnGround()) {
		monster->Add_Velocity(m_dir * BackStepPow,0.f);
		BackStepPow *= 0.2;
	}
	if (!m_bGround && monster->isOnGround()) {
		CResourceMgr::Get_Instance()->PlaySoundW(L"Landing", 0.34f);
		CResourceMgr::Get_Instance()->PlaySoundW(L"Crossbow_Ready", 0.14f);
		monster->Get_Anim()->Set_Stop(false);
		m_bGround = true;
	}
	if (m_bGround&&!m_bFired) {
		if (monster->Get_Anim()->Calc_NowFrame({ 2,1 })) {
			CResourceMgr::Get_Instance()->PlaySoundW(L"Crossbow_Fire", 0.14f);
			CObject* arrow = CObjMgr::Get_Instance()->ReQuestObject<CArrow>(PROJECTILE);
			arrow->Set_Dir(monster->Get_Dir());
			arrow->Initialize();
			arrow->Set_Pos(monster->Get_Collider()->Get_Info().fX, monster->Get_Collider()->Get_Info().fY + 40);
			m_bFired = true;
		}
	}

	if (m_bFired && monster->Get_Anim()->isAnimEnd()) {
		monster->ChangeState(MONSTER_IDLE);
	}
}

bool CArchorAttackB::Exit(CMonster* monster)
{
	monster->Attacked();
	monster->ReleaseTarget();
	return false;
}
