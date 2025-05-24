#include "pch.h"
#include "CBossEnHance.h"
#include "CTimeMgr.h"
#include "CResourceMgr.h"
#include "CEffectMgr.h"

CBossEnHance::CBossEnHance()
	:m_bGround(false), soundID(0)
{
}

CBossEnHance::~CBossEnHance()
{
}

void CBossEnHance::Enter(CBoss* boss)
{
	CResourceMgr::Get_Instance()->StopSound(CH_BGM);

	boss->RestoreHealth();
	boss->Set_Coll_Active(false);
	boss->Set_Name(L"BossC");
	m_bGround = false;
	aniFrame.startPoint = { 0,0 };
	aniFrame.endPoint = { 0,0 };
	//aniFrame.endPoint = { 4,8 };
	aniFrame.isLoop = false;
	aniFrame.frameSpeed = 0.1f;
	aniFrame.m_spSize = { 300,200 };
	boss->Get_Info().Add_OffsetX(50.f, 50.f);
	boss->SetAnim(L"INTRO");
	CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Awakening_Sign", 0.3f);
	CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Scream", 0.2f);
	m_bWaken = false;
	CCamera::Get_Instance()->Cam_Shake(0.8, true, 5);
}

void CBossEnHance::Update(CBoss* boss)
{

	if (boss->isOnGround()) {
		if (!m_bWaken) {
			CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Awakening", 0.3f);
			SetElect(boss);
			m_bWaken = true;
		}
		if (!m_bGround) {
			aniFrame.startPoint = { 1,0 };
			aniFrame.endPoint = { 4,8 };
			aniFrame.frameSpeed = 0.1f;
			boss->SetAnim(L"INTRO");
			m_bGround = true;
		}
	
		if (boss->Get_Anim()->isAnimEnd()) {
			SetFlame(boss);
			boss->ChangeState(BOSS_E_IDLE);
		}
	}
	else {
		boss->Add_Velocity(0.f, GRAVITY * 50);
	}
}

bool CBossEnHance::Exit(CBoss* boss)
{
	boss->Set_Coll_Active(true);
	boss->Get_Info().Add_OffsetX(-50.f, -50.f);
	CResourceMgr::Get_Instance()->PlayBGM(L"Chapter2_Boss_Awaken", 0.15f);
	CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Awakening_Outro", 0.2f);
	CResourceMgr::Get_Instance()->StopSound(soundID);
	return false;
}

void CBossEnHance::SetFlame(CBoss* boss)
{
	CBaseEffect* m_pEffcet = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
	m_pEffcet->Initialize();
	INFO groundInfo = { boss->Get_Info().fX,boss->Get_Collider()->Get_Info().fY, 300, 300};
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 3,3 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 600,600 };
	effectFrame.frameSpeed = 0.1f;

	m_pEffcet->Get_Info().Translate_Info(groundInfo);
	m_pEffcet->PlayAnim(L"Boss_Awaken_effect_Flame", effectFrame);
	m_pEffcet->Set_AutoDeaActive(true);
	m_pEffcet->Set_Layer(POST);

	CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_DarkMeteorGround_Sign", 0.25f);
}

void CBossEnHance::SetElect(CBoss* boss)
{
	CBaseEffect* m_pEffcet = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
	m_pEffcet->Initialize();
	INFO groundInfo = { boss->Get_Info().fX,boss->Get_Collider()->Get_Info().fY, 300, 300 };
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 0,8 };
	effectFrame.isLoop = true;
	effectFrame.LoopPoint = { 0,0 };
	effectFrame.m_spSize = { 300,400 };
	effectFrame.frameSpeed = 0.1f;

	m_pEffcet->Get_Info().Translate_Info(groundInfo);
	m_pEffcet->PlayAnim(L"Boss_Awaken_effect_Elect", effectFrame);
	m_pEffcet->Set_AutoDeaActive(true);
	m_pEffcet->Set_Layer(POST);
	CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Awakening", 0.25f,&soundID,true);
}
