#include "pch.h"
#include "CBossWait.h"
#include "CTimeMgr.h"
#include "CResourceMgr.h"

CBossWait::CBossWait()
	:m_point{}
{
	aniFrame.m_spSize = { 200,200 };
	aniFrame.startPoint = { 0,0 };
	aniFrame.endPoint = { 4,0 };
	aniFrame.isLoop = true;
	aniFrame.LoopPoint = { 0,0 };
	aniFrame.frameSpeed = 0.1f;
	m_bWaiting = true;
}

CBossWait::~CBossWait()
{
}

void CBossWait::Enter(CBoss* boss)
{
	m_bossName = boss->Get_Name();
	m_point = boss->Get_WaitPoint();
	m_elapsedTime = 0;
	boss->Get_Info().Add_OffsetX(-20.f, -20.f);
	boss->Get_Info().Add_OffsetY(-20.f, -20.f);
	boss->Set_Waiting(true);
	boss->Set_Coll_Active(false);

	if (!m_bWaiting) {
		aniFrame.startPoint = { 5,0 };
		aniFrame.endPoint = { 7,0 };
		aniFrame.isLoop = false;
	}
	else {
		aniFrame.startPoint = { 0,0 };
		aniFrame.endPoint = { 4,0 };
		aniFrame.isLoop = true;
		aniFrame.LoopPoint = { 0,0 };
	}
	boss->SetAnim(L"WAIT");
}

void CBossWait::Update(CBoss* boss)
{
	if (!m_bWaiting) { //기다리는 중이 아니면
		if (boss->Get_Collider()->Get_Info().RB().y < 0) { //바닥이 화면 밖에 나가면
			boss->Get_Info().fX = m_point.x; //x좌표 맞춰주고
			boss->Get_Info().fY =-50; 
			boss->Set_Velocity(0.f, 0.f);
			m_bWaiting = true;
		}
		else {
			boss->Add_Velocity(0.f, -20.f);
		}
	}

	if (m_bWaiting) {
		if (boss->Get_Collider()->Get_Info().fY <= m_point.y) {
			boss->Set_Velocity(0.f, GRAVITY*5); //위치에 도달할 때 까지
		}
		else {//위치에 도달했다면
			boss->Set_Velocity(0.f, -0.f); //이건 4.5초 동안 기다리는 중임
			aniFrame.startPoint = { 0,0 };
			aniFrame.endPoint = { 4,0 };
			aniFrame.isLoop = true;
			aniFrame.LoopPoint = { 0,0 };
			boss->SetAnim(L"WAIT");
			m_elapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
			if (m_elapsedTime >5.f) {
				boss->ChangeState(BOSS_IDLE);
			}
		}
	}
}


bool CBossWait::Exit(CBoss* boss)
{
	aniFrame.startPoint = { 5,0 };
	aniFrame.endPoint = { 7,0 };
	boss->SetAnim(L"WAIT");

	boss->Get_Anim()->Set_BreakPoint({ 7,0 });
	boss->Get_Info().Add_OffsetX(20.f, 20.f);
	boss->Get_Info().Add_OffsetY(20.f, 20.f);
	m_bWaiting = false;

	if (m_bossName == L"BossA") {
		boss->Set_Dir(-1);
	}
	else if (m_bossName == L"BossB") {
		boss->Set_Dir(1);
	}
	boss->Add_Velocity(0, 40.f);
	CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Enter", 0.2f);
	m_elapsedTime = 0;

	aniFrame.startPoint = { 0,0 };
	aniFrame.endPoint = { 4,0 };
	aniFrame.isLoop = true;
	return false;
}
