#include "pch.h"
#include "CEnhancedBossGround.h"
#include "CTimeMgr.h"
#include "CEffectMgr.h"
#include "CBaseEffect.h"
#include "CResourceMgr.h"

CEnhancedBossGround::CEnhancedBossGround()
	:m_bReady(false),
	m_bMove(false),
	m_bMoveEnd(false),
	m_pEffcet(nullptr),
	m_bBorder(false),
	m_pLightening{ nullptr },
	m_LightCount(0),
	m_pLightAtk{ nullptr },
	m_lightElapsedTime(0.f)
{
	aniFrame.m_spSize = { 300,200 };
	aniFrame.startPoint = { 0,0 };
	aniFrame.endPoint = { 4,2 };
	aniFrame.isLoop = false;
	aniFrame.LoopPoint = { 0,0 };
	aniFrame.frameSpeed = 0.1f;
	//m_HitBox = new CCollider(this);
}

CEnhancedBossGround::~CEnhancedBossGround()
{
	Safe_Delete<CAttackBox*>(m_pAtkBox);
	for (int i = 0; i < 5; i++) {
		if (m_pLightAtk[i]) {
			Safe_Delete<CAttackBox*>(m_pLightAtk[i]);
		}
	}
}

void CEnhancedBossGround::Enter(CBoss* boss)
{
	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(boss);
		m_pAtkBox->Set_Target(PLAYER);
	}

	for (int i = 0; i < 5; ++i) {
		if (!m_pLightAtk[i]) {
			m_pLightAtk[i] = new CAttackBox(boss);
			m_pLightAtk[i]->Set_Target(PLAYER);
		}
	}

	boss->Get_Info().Add_OffsetX(50.f, 50.f);
	m_bReady = false;
	m_bMove = false;
	m_bMoveEnd = false;
	m_bBorder = false;
	m_lightElapsedTime = 0;

	if (boss->Get_Target()->Get_Info().fX < boss->Get_Info().fX) {
		boss->Set_Dir(-1);
		boss->Get_Info().fX = WINCX - 5;
	}
	else {
		boss->Set_Dir(1);
		boss->Get_Info().fX = 5;
	}

	boss->SetAnim(L"GROUND");
	boss->Get_Anim()->Set_BreakPoint({ 1,1 });
}
void CEnhancedBossGround::Update(CBoss* boss)
{
	
	// 준비 동작 완료 감지
	if (!m_bReady)
	{
		if (boss->Get_Anim()->Calc_NowFrame({ 1,1 })) {// 11포인트 도달하게 되면
			boss->Get_Anim()->Set_Stop(false); //브레이크 풀고
			boss->Get_Anim()->Set_BreakPoint({ 4,1 }); //다음 포인트로
			//여기서 발구름 이펙트 추가
			SMokenEffect(boss);
			m_bReady = true;
		}
		return;
	}
	// 슬라이드 진행 중
	if (!m_bBorder)
	{
		boss->Add_Velocity(250.f * CTimeMgr::Get_Instance()->Get_Delta(), 0);
		if (boss->Get_Dir() == -1 && boss->Get_Info().LT().x <= 15) {
			boss->Set_Velocity(0.f, 0.f);
			m_pAtkBox->Set_DeActive();
			m_bBorder = true;//경계 도달
			CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_DarkMeteor_Impact", 0.25f);
			boss->Get_Anim()->Set_Stop(false);
			return;
		}
		else if (boss->Get_Dir() == 1 && boss->Get_Info().RB().x >= WINCX - 15) {
			boss->Set_Velocity(0.f, 0.f);
			m_pAtkBox->Set_DeActive();
			m_bBorder = true;//경계 도달
			CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_DarkMeteor_Impact", 0.25f);
			boss->Get_Anim()->Set_Stop(false);
			return;
		}
	}

	if (m_bBorder&&!m_bMoveEnd) {//경계 도달했으면
		LinearEffect(boss); // 선 이펙트 
		return;
	}
	
	if (m_bMoveEnd) { //다 이동 했으면
		// 슬라이드 충격 이펙트가 끝나야 번개 패턴 실행
		LightenEffect(boss);
		//애니메이션 제한 해제
	}

	if (m_LightCount >= 5 && boss->Get_Anim()->isAnimEnd()) { //5개 생성하면 끝.
		boss->ChangeState(BOSS_E_IDLE);
	}
}


void CEnhancedBossGround::LinearEffect(CBoss* boss)
{
	m_pAtkBox->Set_Active();
	INFO groundInfo = { WINCX / 2, boss->Get_Info().fY + 80, WINCX, 20 };
	m_pAtkBox->Get_Info().Translate_Info(groundInfo);

	if (!m_pEffcet) { //이펙트 현재 없으면
		m_pEffcet = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
		m_pEffcet->Initialize();
		INFO groundInfo = { WINCX / 2, boss->Get_Info().fY + 50, WINCX + 300, 50 };
		ANI_FRAME effectFrame;
		effectFrame.startPoint = { 0,0 };
		effectFrame.endPoint = { 0,14 };
		effectFrame.isLoop = false;
		effectFrame.m_spSize = { 2400,200 };
		effectFrame.frameSpeed = 0.1f;

		m_pEffcet->Get_Info().Translate_Info(groundInfo);
		m_pEffcet->PlayAnim(L"BossC_Ground_Effect", effectFrame);
		m_pEffcet->Set_AutoDeaActive(true);
		CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_DarkMeteorGround_Sign", 0.25f);
	}
	m_bMoveEnd = true; //이펙트 생성하면 이동 끝
}

bool CEnhancedBossGround::Exit(CBoss* boss)
{
	boss->Get_Info().Add_OffsetX(-50.f, -50.f);
	if (m_pAtkBox->isActive()) {
		m_pAtkBox->Set_DeActive();
	}
	m_pEffcet = nullptr;

	for (int i = 0; i < 5;++i) {
		if (m_pLightening[i]) {
			m_pLightening[i] = nullptr;
			m_pLightAtk[i]->Set_DeActive();
		}
	} //남은거 액티브 취소

	m_LightCount = 0;
	return false;
}

void CEnhancedBossGround::LightenEffect(CBoss* boss)
{
	if (m_LightCount >= 5) return; //5개까지만.

	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 4,3 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 200,300 };
	effectFrame.frameSpeed = 0.04f;

	m_lightElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();

	if (m_lightElapsedTime > 0.5f) {
		m_pLightening[m_LightCount] = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
		INFO groundInfo = { WINCX / 5 * (m_LightCount + 1), WINCY / 2, 100, WINCY };
		m_pLightening[m_LightCount]->Initialize();
		m_pLightening[m_LightCount]->Get_Info().Translate_Info(groundInfo);
		m_pLightening[m_LightCount]->Get_Info().SetFromBottom(boss->Get_Info().RB().y + 20);
		m_pLightening[m_LightCount]->PlayAnim(L"BossC_Judgement_Effect", effectFrame);
		m_pLightAtk[m_LightCount]->Get_Info().Translate_Info(groundInfo);
		m_pLightAtk[m_LightCount]->Set_Active();
		m_LightCount += 1; //0부터 
		m_lightElapsedTime = 0;
		CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_DarkMeteorGround_Thunder", 0.15f * m_LightCount);
	}
}

void CEnhancedBossGround::SMokenEffect(CBoss* boss)
{
	CBaseEffect* m_pEffcet = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(HAS_DIRECTION));
	m_pEffcet->Initialize();
	INFO groundInfo = { boss->Get_Info().fX,boss->Get_Collider()->Get_Info().fY, 300, 300 };
	ANI_FRAME effectFrame;
	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 2,4 };
	effectFrame.isLoop = false;
	effectFrame.m_spSize = { 150,100 };
	effectFrame.frameSpeed = 0.1f;

	m_pEffcet->Get_Info().Translate_Info(groundInfo);
	m_pEffcet->Set_Dir(boss->Get_Dir());
	m_pEffcet->PlayAnim(L"Boss_Enhance_MeteorSmoke", effectFrame);
	m_pEffcet->Set_AutoDeaActive(true);
	m_pEffcet->Set_Layer(POST);
}
