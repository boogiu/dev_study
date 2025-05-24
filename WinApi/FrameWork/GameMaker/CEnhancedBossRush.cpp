#include "pch.h"
#include "CEnhancedBossRush.h"
#include "CTimeMgr.h"
#include "CBaseEffect.h"
#include "CEffectMgr.h"
#include "CObjMgr.h"
#include "CDimensionPierce.h"
#include "CAttackBox.h"
#include "CResourceMgr.h"

CEnhancedBossRush::CEnhancedBossRush() : m_patternFlag(6, false), m_elapseTime(0.f), m_bActiveAll(false), m_patternDelay(0.f), AttackCount(0), m_AtkBox(nullptr)
{
	aniFrame.startPoint = { 00 };
	aniFrame.endPoint = { 2,5 };
	aniFrame.m_spSize = { 300, 200 };
	aniFrame.isLoop = false;
	aniFrame.frameSpeed = 0.1f;
}

CEnhancedBossRush::~CEnhancedBossRush()
{
	Safe_Delete<CAttackBox*>(m_AtkBox);
}

void CEnhancedBossRush::Enter(CBoss* boss)
{
	boss->Get_Info().Add_OffsetX(50.f, 50.f);
	boss->SetAnim(L"RUSH");
	m_Laservec = {
	{ { WINCX / 2 - 180,			WINCY / 2,				20, WINCY },				0.f,		{3,0}, false, nullptr, 0.f }, //1
	{ { WINCX / 2 +390,			WINCY / 2 + 10,		20, WINCY },		0.5f,	{0,1}, false, nullptr, 0.f },//2
	//{ { WINCX / 2 - 100,			WINCY / 2 + 40,	20, WINCY },		-1.5f, {3,1}, false, nullptr, 0.f },//3
	{ { WINCX / 2 + 250,			WINCY / 2 - 20,		20, WINCY },		5.5f,	{5,1}, false, nullptr, 0.f },//4
	{ { WINCX / 2 - 50,			WINCY / 2 - 40,		20, WINCY },		1.0f,	{5,1}, false, nullptr, 0.f },//5
	//{ { WINCX / 2 + 100,			WINCY / 2 + 30,		20, WINCY },		-1.0f,	{5,1}, false, nullptr, 0.f },//6
	{ { WINCX / 2 + 550,			WINCY / 2 + 15,		20, WINCY },		4.0f,	{5,1}, false, nullptr, 0.f },//7
	{ { WINCX / 2 + 40,	WINCY / 2 + 5,			20, WINCY },	-2.0f,	{5,1}, false, nullptr, 0.f },//8
	};
	m_elapseTime = 0.f;
	m_patternDelay = 0.f;
	m_bActiveAll = false;
	AttackCount = 0;
	m_eNowState = ENTER;
	boss->Get_Anim()->Set_Stop(true);

	if (!m_AtkBox) {
		m_AtkBox = new CAttackBox(boss);
	}
	m_AtkBox->Set_Target(PLAYER);
	prevDir = 0;
	AttackSoundA = false;
	AttackSoundB= false;
	m_telPor_in = false;
}

// Update 내부 (위치 통일 & 간결하게 리팩터링)
void CEnhancedBossRush::Update(CBoss* boss)
{
	// 방향 설정
	if (boss->Get_Target()->Get_Info().fX < boss->Get_Info().fX)
		boss->Set_Dir(-1);
	else
		boss->Set_Dir(1);
	
	switch (m_eNowState)
	{
	case CEnhancedBossRush::ENTER:
		MoveToTarget(boss);
		break;
	case CEnhancedBossRush::CHASETARGET:
		MoveToTarget(boss);
		break;
	case CEnhancedBossRush::FIRSTATK:
		FirstAtk(boss);
		break;
	case CEnhancedBossRush::SECONDATK:
		SecontAtk(boss);
		break;
	case CEnhancedBossRush::LASERACTIVE:
		Active_All(boss);
		break;
	case CEnhancedBossRush::LASERFIRE:
		m_elapseTime += CTimeMgr::Get_Instance()->Get_Delta();

		if (m_elapseTime > 0.5f) {
			Fire_All(boss);
		}
		break;
	case CEnhancedBossRush::ENDSTATE:
		break;
	default:
		break;
	}
	
	if (boss->Get_Anim()->isAnimEnd()&&m_eNowState==ENDSTATE) {
		boss->ChangeState(BOSS_E_IDLE);
	}
}

bool CEnhancedBossRush::Exit(CBoss* boss)
{
	boss->Get_Info().Add_OffsetX(-50.f, -50.f);
	//m_Laservec.clear();
	boss->Set_Render(true);
	return false;
}

void CEnhancedBossRush::MoveToTarget(CBoss* boss)
{
	m_patternDelay += CTimeMgr::Get_Instance()->Get_Delta();
	if (!m_telPor_in) {
		CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Teleport_In", 0.2f);
		m_telPor_in = true;
		Smoke(boss);
	}

	if (m_patternDelay < 0.8f) {
		boss->Set_Render(false);
	}
	else {
		int distance = 100;
		if (prevDir==0) {
			boss->Get_Info().fX = boss->Get_Target()->Get_Info().fX - (distance * boss->Get_Dir());
			prevDir = boss->Get_Dir();
		}
		else {
			boss->Get_Info().fX = boss->Get_Target()->Get_Info().fX - (distance * prevDir*-1);
			prevDir *= -1;
			boss->Set_Dir(prevDir);
		}
		boss->Change_anim_path(L"RUSH");
		//움직이고 애니메이션 작동.
		if (AttackCount == 0) {
			m_eNowState = FIRSTATK;
		}
		else if (AttackCount == 1) {
			m_eNowState = SECONDATK;
		}
		else if (AttackCount == 2) {
			m_eNowState = LASERACTIVE;
		}
		CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Teleport_Out", 0.2f);
		Smoke(boss);
		m_patternDelay = 0;
		m_telPor_in = false;
	}
	
}

void CEnhancedBossRush::Fire_All(CBoss* boss)
{
	boss->Get_Anim()->Set_Stop(false);
	CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_DarkDimensionPierce_Impact", 0.3f);
	for (auto& laser : m_Laservec) {
		laser.pLaser->Set_Fire();
	}
	CCamera::Get_Instance()->Cam_Shake(0.2f, false, 5);
	m_eNowState = ENDSTATE;
}

void CEnhancedBossRush::Active_All(CBoss* boss)
{
	boss->Get_Anim()->Set_Stop(false);
	boss->Set_Render(true);

	m_elapseTime += CTimeMgr::Get_Instance()->Get_Delta();
	if (m_elapseTime > 0.3f) {
		for (auto& laser : m_Laservec) {
			if (laser.bFired == true) continue;
			else {
				laser.pLaser = Create_Laser(boss);
				laser.pLaser->Set_Angle(laser.angleRad);
				laser.pLaser->Get_Info().Translate_Info(laser.info);
				laser.bFired = true;
				m_elapseTime = 0;
				break;
			}
		}
	}

	if (all_of(m_Laservec.begin(), m_Laservec.end(), [](const tagLaserEvent& evt) {
		return evt.bFired;
		})) {
		m_bActiveAll = true;
		m_elapseTime = 0;
	}

	if (boss->Get_Anim()->Calc_NowFrame({ 2,4 })) {
		boss->Get_Anim()->Set_Stop(true);
	}

	if (m_bActiveAll) {
		m_eNowState = LASERFIRE;
		m_elapseTime = 0;
	}
}

CDimensionPierce* CEnhancedBossRush::Create_Laser(CBoss* boss)
{
	INFO_EX targetInfo = boss->Get_Target()->Get_Info();
	CObject* Laser = CObjMgr::Get_Instance()->ReQuestObject<CDimensionPierce>(PROJECTILE);
	CDimensionPierce* CastLaser = static_cast<CDimensionPierce*>(Laser);
	CastLaser->Initialize();
	return CastLaser;
}

void CEnhancedBossRush::FirstAtk(CBoss* boss)
{
	//시작
	INFO atkPos = { boss->Get_Info().fX + (boss->Get_Dir() * 80), boss->Get_Info().fY,  120.f, 200.f };
	m_AtkBox->Get_Info().Translate_Info(atkPos);
	m_AtkBox->Set_Target(PLAYER);
	m_AtkBox->Set_Active();
	boss->Get_Anim()->Set_Stop(false);
	boss->Set_Render(true);
	boss->Set_Coll_Active(false);

	if (boss->Get_Anim()->Calc_NowFrame({ 4,0 })) {
		AttackCount += 1;
		m_eNowState = CHASETARGET;
		boss->Get_Anim()->Set_Stop(true);
		m_AtkBox->Set_DeActive();
		boss->Set_Coll_Active(true);
		if (!AttackSoundA) {
			CResourceMgr::Get_Instance()->PlaySoundW(L"	Leiana_DarkDimensionRush_Sign", 0.3f);
			AttackSoundA = true;
		}
	}
	//끝
}

void CEnhancedBossRush::SecontAtk(CBoss* boss)
{
	//시작
	INFO atkPos = { boss->Get_Info().fX + (boss->Get_Dir() * 80), boss->Get_Info().fY,  120.f, 200.f };
	m_AtkBox->Get_Info().Translate_Info(atkPos);
	m_AtkBox->Set_Target(PLAYER);
	m_AtkBox->Set_Active();
	boss->Get_Anim()->Set_Stop(false);
	boss->Set_Render(true);
	boss->Set_Coll_Active(false);
	//끝
	if (boss->Get_Anim()->Calc_NowFrame({ 1,1 })) {
		AttackCount += 1;
		m_eNowState = CHASETARGET;
		boss->Get_Anim()->Set_Stop(true);
		m_AtkBox->Set_DeActive();
		boss->Set_Coll_Active(true);
		if (!AttackSoundB) {
			CResourceMgr::Get_Instance()->PlaySoundW(L"	Leiana_DarkDimensionRush_Sign", 0.3f);
			AttackSoundB = true;
		}
	}
}

void CEnhancedBossRush::Smoke(CBoss* boss) {
	CBaseEffect* m_pEffcet = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(HAS_DIRECTION));
	m_pEffcet->Initialize();

	INFO groundInfo = { boss->Get_Info().fX -100* boss->Get_Dir(),boss->Get_Collider()->Get_Info().RB().y - 50, 150, 100};
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 2,4 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 150,100 };
	effectFrame.frameSpeed = 0.1f;

	m_pEffcet->Get_Info().Translate_Info(groundInfo);

	m_pEffcet->Set_Dir(boss->Get_Dir());
	m_pEffcet->PlayAnim(L"BigDash_Smoke", effectFrame);
	m_pEffcet->Set_AutoDeaActive(true);
	m_pEffcet->Set_Layer(POST);
}