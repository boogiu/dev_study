#include "pch.h"
#include "CBossIntro.h"
#include "CTimeMgr.h"
#include "CResourceMgr.h"

CBossIntro::CBossIntro()
	:m_bJumped(false), m_bGround(false)
{
	aniFrame.m_spSize = { 600,200 };
	aniFrame.startPoint = { 1,0 };
	aniFrame.endPoint = { 3,0};
	aniFrame.isLoop = false;
	aniFrame.LoopPoint = { 0,0 };
	aniFrame.frameSpeed = 0.1f;
}

CBossIntro::~CBossIntro()
{
}

void CBossIntro::Enter(CBoss* boss)
{
	boss->Get_Info().Add_OffsetX(200.f,200.f);
	boss->SetAnim(L"INTRO");
	boss->Set_Coll_Active(false);
	boss->Set_Velocity(0.f, -150);
	m_bTakeOff = false;
}

void CBossIntro::Update(CBoss* boss)
{
	if (boss->isOnGround()) {
		boss->Set_Velocity(0.f, 0.f);
		if(!m_bGround){
			aniFrame.startPoint = { 3,0 };
			aniFrame.endPoint = { 4,6 };
			boss->SetAnim(L"INTRO");
			m_bGround = true;
			CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_FootStep", 0.2f);
		}
		if (boss->Get_Anim()->Calc_NowFrame({1,4})) {
			if (!m_bTakeOff) {
				CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Intro_TakeOff", 0.2f);
				m_bTakeOff = true;
			}
		}
		if (boss->Get_Anim()->isAnimEnd()) {
			boss->Set_Coll_Active(true);
			boss->ChangeState(BOSS_IDLE);
		}
	}
	else{
		boss->Add_Velocity(0.f,GRAVITY);
	}
}

bool CBossIntro::Exit(CBoss* boss)
{
	CResourceMgr::Get_Instance()->PlaySoundW(L"	Leiana_Intro_Impact", 0.3f);
	boss->Get_Info().Add_OffsetX(-200.f, -200.f);
	boss->Set_Waiting(false);
	return false;
}

void CBossIntro::ChangeIntro()
{
}
