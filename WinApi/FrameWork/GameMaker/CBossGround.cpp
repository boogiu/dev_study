#include "pch.h"
#include "CBossGround.h"
#include "CTimeMgr.h"
#include "CBaseEffect.h"
#include "CEffectMgr.h"
#include "CAttackBox.h"
#include "CResourceMgr.h"

CBossGround::CBossGround()
	:m_Dir(0), m_bSmoke(false), m_AtkBox(nullptr)
{
	aniFrame.m_spSize = { 300,200 };
	aniFrame.startPoint = { 0,0 };
	aniFrame.endPoint = { 8,0 };
	aniFrame.isLoop = false;
	aniFrame.LoopPoint = { 0,0 };
	aniFrame.frameSpeed = 0.15f;
}

CBossGround::~CBossGround()
{
	Safe_Delete<CAttackBox*>(m_pAtkBox);
}

void CBossGround::Enter(CBoss* boss)
{
	boss->Get_Info().Add_OffsetX(50.f, 50.f);
	m_bReady = false;
	m_bMoved = false;
	m_ReadyTime = 0.f;

	if (boss->Get_Target()->Get_Info().fX < boss->Get_Info().fX) {
		boss->Set_Dir(-1);
	}
	else {
		boss->Set_Dir(1);
	}
	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(boss);
		m_pAtkBox->Set_Target(PLAYER);
	}
	boss->SetAnim(L"GROUND");
	boss->Get_Anim()->Set_BreakPoint({ 0,6 });
	m_bSmoke = false;
	m_MoveEnd = false;
}

void CBossGround::Update(CBoss* boss)
{
	m_ReadyTime += CTimeMgr::Get_Instance()->Get_Delta();
	if (m_ReadyTime > 0.3f && !m_bReady) {
		m_bReady = true;
		m_ReadyTime = 0;
		boss->Get_Anim()->Set_BreakPoint({ 0,8 });
	}

	//이동
	if (m_bReady && !m_bMoved && !m_MoveEnd) {
		Smoke(boss);
		m_pAtkBox->Set_Active();
		INFO groundInfo = { boss->Get_Info().fX, boss->Get_Info().fY + 80, 500, 20 };
		boss->Add_Velocity(220.f * CTimeMgr::Get_Instance()->Get_Delta(), 0);
		m_pAtkBox->Get_Info().Translate_Info(groundInfo);
	}

	if (!m_MoveEnd) {
		if (boss->Get_Dir() == -1) {//왼쪽으로 가면
			if (boss->Get_Collider()->Get_Info().LT().x <= 25) {
				boss->Set_Velocity(0.f, 0.f);
				m_MoveEnd = true;
				m_bMoved = true;
				m_pAtkBox->Set_DeActive();
				CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_MeteorGround", 0.3f);
				boss->Get_Anim()->Set_Stop(false);
				LinearEffect(boss);
			}
		}

		else if (boss->Get_Dir() == 1) {//왼쪽으로 가면
			if (boss->Get_Collider()->Get_Info().RB().x >= WINCX - 25) {
				boss->Set_Velocity(0.f, 0.f);
				aniFrame.startPoint = { 0,1 };
				aniFrame.endPoint = { 8,1 };
				aniFrame.isLoop = false;

				boss->SetAnim(L"GROUND");
				m_MoveEnd = true;
				m_bMoved = true;
				m_pAtkBox->Set_DeActive();
				CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_MeteorGround", 0.3f);
				boss->Get_Anim()->Set_Stop(false);
				LinearEffect(boss);
			}
		}
		return;
	}

	if (boss->Get_Anim()->isAnimEnd()) {
		if (m_bReady && m_bMoved && m_MoveEnd) {
			boss->ChangeState(BOSS_IDLE);
			return;
		}
	}
}

bool CBossGround::Exit(CBoss* boss)
{
	boss->Get_Info().Add_OffsetX(-50.f, -50.f);

	return false;
}

void CBossGround::Smoke(CBoss* boss)
{
	if (m_bSmoke) return;
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 1,2 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 150,100 };
	effectFrame.frameSpeed = 0.1f;

	CBaseEffect* Effect = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(HAS_DIRECTION));
	Effect->Initialize();
	INFO groundInfo = { boss->Get_Info().fX, boss->Get_Collider()->Get_Info().RB().y - 50,150,150 };
	Effect->Get_Info().Translate_Info(groundInfo);
	Effect->Set_Dir(boss->Get_Dir());
	Effect->PlayAnim(L"Boss_MeteorSmoke", effectFrame);
	Effect->Set_AutoDeaActive(true);
	CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_MeteorGround_Ready", 0.3f);
	m_bSmoke = true;
}

void CBossGround::LinearEffect(CBoss* boss)
{
	CBaseEffect* m_pEffcet = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
	m_pEffcet->Initialize();
	INFO groundInfo = { WINCX / 2, boss->Get_Info().fY + 30, WINCX + 300, 50 };
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 0,5 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 400,100 };
	effectFrame.frameSpeed = 0.1f;

	m_pEffcet->Get_Info().Translate_Info(groundInfo);
	m_pEffcet->PlayAnim(L"Ground_Sign", effectFrame);
	m_pEffcet->Set_AutoDeaActive(true);
	CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_DarkMeteorGround_Sign", 0.25f);
}