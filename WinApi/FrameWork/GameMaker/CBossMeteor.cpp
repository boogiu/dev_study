#include "pch.h"
#include "CBossMeteor.h"
#include "CTimeMgr.h"
#include "CBaseEffect.h"
#include "CEffectMgr.h"
#include "CAttackBox.h"
#include "CResourceMgr.h"

CBossMeteor::CBossMeteor()
	:m_bDown(false), m_bReady(false), m_bUp(false), m_AtkBox(nullptr)
{
	aniFrame.frameSpeed = 0.1f;
	aniFrame.m_spSize = { 300,200 };
	aniFrame.isLoop = false;
	aniFrame.LoopPoint = { 0,0 };
}

CBossMeteor::~CBossMeteor()
{
	Safe_Delete<CAttackBox*>(m_pAtkBox);
}

void CBossMeteor::Enter(CBoss* boss)
{
	m_bDown	=false; 
	m_bReady	=false;
	m_bUp			=false;
	//점프
	aniFrame.startPoint = { 0,0 };
	aniFrame.endPoint = { 3,0 };

	boss->Get_Info().Add_OffsetX(50.f, 50.f);
	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(boss);
	}
	m_pAtkBox->Set_Target(PLAYER);
	m_pAtkBox->Set_Active();
	boss->SetAnim(L"METEOR");
	CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_GoldenMeteor", 0.2f);
	m_bEffected = false;
	MeteorEffect = nullptr;
}

void CBossMeteor::Update(CBoss* boss)
{

	INFO groundInfo = {
		boss->Get_Collider()->Get_Info().fX,
		boss->Get_Collider()->Get_Info().fY,
		boss->Get_Collider()->Get_Info().X_Size(),
		boss->Get_Collider()->Get_Info().Y_Size() };

	m_pAtkBox->Get_Info().Translate_Info(groundInfo);

	if (!m_bUp) {
		boss->Add_Velocity(0.f, -50 * CTimeMgr::Get_Instance()->Get_Delta());

		if (boss->Get_Info().RB().y <= 0) {
			boss->Get_Info().fX = boss->Get_Target()->Get_Info().fX;
			m_bUp = true;
			//회전
			aniFrame.startPoint = { 0,1 };
			aniFrame.endPoint = { 3,1 };
			boss->SetAnim(L"METEOR");
			m_bReady = true;
		}
		return;
	}

	if (m_bReady) {
		boss->Set_Velocity(0.f, 0.f);
		if (boss->Get_Anim()->isAnimEnd()) {
			//하강
			aniFrame.startPoint = { 0,2 };
			aniFrame.endPoint = { 1,2 };
			boss->SetAnim(L"METEOR");
			m_bReady = false;
			m_bDown = true;
		}
		return;
	}

	if (m_bDown) {
		boss->Add_Velocity(0.f, 150);
		Meteor_Effect(boss);

		if (boss->isOnGround()) {
			//착지
			boss->Set_Velocity(0.f, 0.f);
			CBaseEffect* Effect = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
			Effect->Initialize();
			INFO groundInfo = { boss->Get_Info().fX, boss->Get_Collider()->Get_Info().RB().y,200,100};
			ANI_FRAME effectFrame;
			effectFrame.startPoint = { 0,0 };
			effectFrame.endPoint = { 1,4 };
			effectFrame.isLoop = false;
			effectFrame.m_spSize = { 400,200 };
			effectFrame.frameSpeed = 0.01f;
			Effect->Get_Info().Translate_Info(groundInfo);
			Effect->PlayAnim(L"Boss_Ground_Smoke", effectFrame);
			Effect->Set_AutoDeaActive(true);

			aniFrame.startPoint = { 2,2 };
			aniFrame.endPoint = { 3,4 };
			boss->SetAnim(L"METEOR");
			m_bDown = false;
			m_pAtkBox->Set_DeActive();
			CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Landing", 0.2f);
			MeteorEffect->Set_Active(false);
		}
		return;
	}

	if (boss->Get_Anim()->isAnimEnd()) {

		boss->ChangeState(BOSS_IDLE);
	}
}

bool CBossMeteor::Exit(CBoss* boss)
{
	boss->Get_Info().Add_OffsetX(-50.f, -50.f);
	return false;
}

void CBossMeteor::Meteor_Effect(CBoss* boss)
{
	if (!m_bEffected) {
		MeteorEffect = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
		MeteorEffect->Initialize();
		INFO groundInfo = { boss->Get_Info().fX, boss->Get_Collider()->Get_Info().RB().y- 20,50,150 };
		ANI_FRAME effectFrame;
		effectFrame.startPoint = { 0,0 };
		effectFrame.endPoint = { 1,1 };
		effectFrame.isLoop = true;
		effectFrame.LoopPoint = { 0,0 };
		effectFrame.m_spSize = { 50,300 };
		effectFrame.frameSpeed = 0.01f;
		MeteorEffect->Get_Info().Translate_Info(groundInfo);
		MeteorEffect->PlayAnim(L"Golden_Meteor_Effect", effectFrame);
		MeteorEffect->Set_AutoDeaActive(false);
		m_bEffected = true;
	}
	MeteorEffect->Get_Info().fY = boss->Get_Collider()->Get_Info().RB().y - 20;
}
