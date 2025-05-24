#include "pch.h"
#include "CEnhancedBossMeteor.h"
#include "CTimeMgr.h"
#include "CObjMgr.h"
#include "CMeteorBullet.h"
#include "CEnhancedBossGround.h"
#include "CResourceMgr.h"
#include "CEffectMgr.h"

CEnhancedBossMeteor::CEnhancedBossMeteor()
	:m_bDown(false), m_bReady(false), m_bUp(false)
{
	aniFrame.frameSpeed = 0.1f;
	aniFrame.m_spSize = { 300,300 };
	aniFrame.isLoop = false;
	aniFrame.LoopPoint = { 2,4 };
	aniFrame.startPoint = { 0,0 };
	aniFrame.endPoint = { 4,3 };
}

CEnhancedBossMeteor::~CEnhancedBossMeteor()
{
}

void CEnhancedBossMeteor::Enter(CBoss* boss)
{
	m_bDown = false;
	m_bReady = false;
	m_bUp = false;

	boss->Get_Info().Add_OffsetX(50.f, 50.f);
	boss->Get_Info().Add_OffsetY(100.f, 0.f);
	boss->SetAnim(L"METEOR");
	boss->Get_Anim()->Set_BreakPoint({3,0});
	CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_GoldenMeteor", 0.2f);
}

void CEnhancedBossMeteor::Update(CBoss* boss)
{
	if (!m_bUp) {
		boss->Add_Velocity(0.f, -50 * CTimeMgr::Get_Instance()->Get_Delta());
		if (boss->Get_Info().RB().y <= 10) { //위로 올라가면 위치 이동 
			boss->Get_Info().fX = boss->Get_Target()->Get_Info().fX;
	
			boss->Get_Anim()->Set_Stop(false);
			boss->Get_Anim()->Set_BreakPoint({ 2,1 });
			if (boss->Get_Anim()->Calc_NowFrame({ 2,1 })) {
				m_bUp = true;
				m_bReady = true;
			}
		}
		return;
	}

	if (m_bReady) {
		boss->Set_Velocity(0.f, 0.f);
		boss->Get_Anim()->Set_Stop(false);
		boss->Get_Anim()->Set_BreakPoint({ 4,1 });
		if (boss->Get_Anim()->Calc_NowFrame({ 4,1 })) {
			//하강
			m_bReady = false;
			m_bDown = true;
			Sign(boss);
		}
		return;
	}

	if (m_bDown) {
		boss->Add_Velocity(0.f, 100);
		if (boss->isOnGround()) {
			CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_DarkMeteor_Impact", 0.3f);
			boss->Get_Anim()->Set_Stop(false);
			boss->Set_Velocity(0.f, 0.f);
			CObject* tmp1 = CObjMgr::Get_Instance()->ReQuestObject<CMeteorBullet>(PROJECTILE);
			tmp1->Set_Dir(-1);
			tmp1->Initialize();
			tmp1->Get_Info().SetFromBottom(boss->Get_Collider()->Get_Info().RB().y );
			tmp1->Get_Info().fX = boss->Get_Info().fX;

			CObject* tmp2 = CObjMgr::Get_Instance()->ReQuestObject<CMeteorBullet>(PROJECTILE);
			tmp2->Set_Dir(1);
			tmp2->Initialize();
			tmp2->Get_Info().fX = boss->Get_Info().fX;
			tmp2->Get_Info().SetFromBottom(boss->Get_Collider()->Get_Info().RB().y );
			m_bDown = false;
			Impact(boss);
			Impact(boss);
		}
		return;
	}
	if (boss->Get_Anim()->isAnimEnd()) {
		boss->ChangeState(BOSS_E_IDLE);
	}
}

bool CEnhancedBossMeteor::Exit(CBoss* boss)
{
	boss->Get_Info().Add_OffsetX(-50.f, -50.f);
	boss->Get_Info().Add_OffsetY(-100.f, 0.f);

	return false;
}

void CEnhancedBossMeteor::Sign(CBoss* boss)
{
	CBaseEffect* m_pEffcet = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
	m_pEffcet->Initialize();

	INFO groundInfo = { boss->Get_Info().fX,WINCY/2, 50, WINCY };
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 8,0 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 80,750 };
	effectFrame.frameSpeed = 0.1f;

	m_pEffcet->Get_Info().Translate_Info(groundInfo);

	m_pEffcet->Set_Dir(boss->Get_Dir());
	m_pEffcet->PlayAnim(L"Dark_meteor_Sign", effectFrame);
	m_pEffcet->Set_AutoDeaActive(true);
	m_pEffcet->Set_Layer(POST);
}

void CEnhancedBossMeteor::Impact(CBoss* boss)
{
	CBaseEffect* m_pEffcet = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
	m_pEffcet->Initialize();

	INFO groundInfo = { boss->Get_Info().fX,boss->Get_Collider()->Get_Info().RB().y-100, 150,200};
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = {3,2 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 150,200 };
	effectFrame.frameSpeed = 0.1f;

	m_pEffcet->Get_Info().Translate_Info(groundInfo);

	m_pEffcet->Set_Dir(boss->Get_Dir());
	m_pEffcet->PlayAnim(L"Dark_meteor_Impact", effectFrame);
	m_pEffcet->Set_AutoDeaActive(true);
	m_pEffcet->Set_Layer(POST);
}

void CEnhancedBossMeteor::Elect(CBoss* boss)
{
	CBaseEffect* m_pEffcet = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
	m_pEffcet->Initialize();

	INFO groundInfo = { boss->Get_Info().fX,boss->Get_Collider()->Get_Info().fY, 150,85 };
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 1,2 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 120,70 };
	effectFrame.frameSpeed = 0.1f;

	m_pEffcet->Get_Info().Translate_Info(groundInfo);

	m_pEffcet->Set_Dir(boss->Get_Dir());
	m_pEffcet->PlayAnim(L"Dark_meteor_Ground", effectFrame);
	m_pEffcet->Set_AutoDeaActive(true);
	m_pEffcet->Set_Layer(POST);
}
