#include "pch.h"
#include "CBossStage.h"
#include "CCamera.h"
#include "CResourceMgr.h"
#include "CObject.h"
#include "CObjMgr.h"
#include "CBossBackGround.h"
#include "CTimeMgr.h"
#include "CFunction.h"
#include "CAnim.h"
#include "CKeyMgr.h"
#include "CUIMgr.h"

CBossStage::CBossStage()
	: m_WaitPtA{ WINCX/2 -280,220 }, m_WaitPtB{ WINCX / 2 +280, 220 }, m_IntroTime(0.f),
	m_pBoss1(nullptr), m_pBoss2(nullptr), m_pBoss3(nullptr), m_pBackBoss(nullptr), m_pUI(nullptr),
	m_bIntroEnd(false), m_patternTime(0.f), m_bStopPattern(false), m_b2phase(false), m_Bgm(0.2f)
{

}

CBossStage::~CBossStage()
{
	Release();
}

void CBossStage::Initialize()
{
	//타일 데이터 로드
	__super::Load_Map(L"../TileData/BossStage.dat");
	//화면으로 보더 고정
	CCamera::Get_Instance()->Reset_Border();
	//
	CResourceMgr::Get_Instance()->StopAll();

	//보더에 카메라 무빙 제한
	CObject* player = CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER);
	player->Set_MoveSCLimit(true);
	player->Set_Pos(WINCX / 2, WINCY / 2);

	//보스 생성 2개 보스
	m_pBoss1 = static_cast<CBoss*>(CObjMgr::Get_Instance()->ReQuestObject<CBoss>(MONSTER));
	m_pBoss2 = static_cast<CBoss*>(CObjMgr::Get_Instance()->ReQuestObject<CBoss>(MONSTER));

	m_pBoss1->Set_BossType(1);
	m_pBoss1->Initialize();
	m_pBoss1->Set_Pos(m_WaitPtB.x, m_WaitPtB.y);
	m_pBoss1->Set_WaitPoint(m_WaitPtB); //초기 위치 설정
	m_pBoss1->Set_Target(player);

	m_pBoss2->Set_BossType(2);
	m_pBoss2->Initialize();
	m_pBoss2->Set_Pos(m_WaitPtA.x, m_WaitPtA.y);
	m_pBoss2->Set_WaitPoint(m_WaitPtA);//초기 위치 설정
	m_pBoss2->Set_Target(player);


	//보스 뒷배경 생성
	m_pBackBoss = new CBossBackGround;
	m_pBackBoss->Initialize();

	//---UI 요청---------------------------------------//
	m_pUI = static_cast<CBossFrame*>(CUIMgr::Get_Instance()->Request_UI<CBossFrame>());
	m_pUI->Initialize();
	m_bBoss3DeadHandled = false;
	//__super::Set_Player_Pos();
	m_bBossDead = false;
	m_bAppearBox = false;
	m_bbackRun = false;
	m_DeadTime = 0;
	CCamera::Get_Instance()->Cinematic(L"황금 갈기 기사단 부관\n 레이아나 자매", 5.f);

}

void CBossStage::Update()
{
	for (int i = 0; i < (int)TILE_END;++i) {
		for (CObject* tile : m_StageTile[(TILE_TYPE)i]) {
			tile->Update();
		}
	}

	m_pBackBoss->Update();

	//HP CHECK 
	Check_Boss_Hp();

	//INTRO 
	if (!m_bIntroEnd) {
		StartIntro();
	}

	//PATTERN
	if (m_bIntroEnd && !m_bStopPattern) {
		if (!m_b2phase) {
			Phase1Pattern();
		}
		else {
			Phase2Pattern();
		}
	}

	//HP_CHECK WITH UI
	if (!m_b2phase) {
		m_pUI->CheckBossAHP(m_pBoss1->ReturnHpRatio());
		m_pUI->CheckBossBHP(m_pBoss2->ReturnHpRatio());
	}

	else {
		if (!m_bBossDead) {
			m_pBackBoss->SetAnim(BOSS_IDLE);
		}
		m_pUI->CheckBossCHP(m_pBoss3->ReturnHpRatio());
	}

	if (m_bBossDead) {
		m_DeadTime += CTimeMgr::Get_Instance()->Get_Delta();
		// 선형으로 볼륨 감소
		m_Bgm -= CTimeMgr::Get_Instance()->Get_Delta() * 0.2f; // 감쇠 속도 조절 (0.2f는 감쇠계수)
		m_Bgm = max(0.f, m_Bgm); // 0 이하로 떨어지지 않게
		CResourceMgr::Get_Instance()->SetChannelVolume(CH_BGM, m_Bgm);

		if (m_DeadTime > 1.f) {
			m_pBackBoss->SetAnim(BOSS_E_DEAD);
			if (!m_bbackRun) {
				CResourceMgr::Get_Instance()->PlaySoundW(L"Leiana_Outro_ArmorStep", 0.2f);
				m_bbackRun = true;
			}
		}
	
		if (!m_pBackBoss->Is_Activate()) {
			if (!m_bAppearBox) {
				CObject* box = CObjMgr::Get_Instance()->ReQuestObject<CStageBox>(ITEM);
				box->Initialize();
				m_bAppearBox = true;
			}
		}
	}
}

void CBossStage::Late_Update()
{
	m_pBackBoss->Late_Update();
}

void CBossStage::Render(HDC _hDC)
{
	auto pCam = CCamera::Get_Instance();
	SIZE sz = pCam->Get_CamSize();
	memDC = CResourceMgr::Get_Instance()->Find_Image(L"Boss_Back");
	TransparentBlt(_hDC, 0, 0, sz.cx, sz.cy, memDC, 150, 0, 600, 400, HOLLOW);

	for (int i = 0; i < (int)TILE_END;++i) {
		for (CObject* tile : m_StageTile[(TILE_TYPE)i]) {
			if (CCamera::Get_Instance()->Out_of_Camera(tile->Get_Info())) {
				continue;
			}
			tile->Render(_hDC);
		}
	}
	m_pBackBoss->Render(_hDC);
}

void CBossStage::Release()
{
	CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER)->Set_MoveSCLimit(false);

	Safe_Delete<CBossBackGround*>(m_pBackBoss);
}

void CBossStage::StartIntro()
{
	m_IntroTime += CTimeMgr::Get_Instance()->Get_Delta();
	if (m_IntroTime > 6.f) {
		m_pBoss1->ChangeState(BOSS_INTRO);
		m_pBoss2->ChangeState(BOSS_INTRO);
		CResourceMgr::Get_Instance()->PlayBGM(L"Chapter2_Boss", 0.2f);
		m_bIntroEnd = true;
		m_IntroTime = 0;
	}
}

void CBossStage::Phase1Pattern()
{
	m_patternTime += CTimeMgr::Get_Instance()->Get_Delta();
	if (m_patternTime < 4.f) { return; }	//3초룰

	BOSS_STATE A_Pattern;
	BOSS_STATE B_Pattern;

	BOSS_STATE A_PrevPattern= m_pBoss1->Get_State();
	BOSS_STATE B_PrevPattern=m_pBoss2->Get_State();

	if (m_pBoss1->Get_State() == BOSS_IDLE&&m_pBoss1->Get_IDLE_END()) {
		A_Pattern = (BOSS_STATE)CFunction::GetInt(4, (int)BOSS_ENHANCE - 1);
		if (A_PrevPattern == BOSS_WAIT && A_Pattern == BOSS_WAIT) {
			A_Pattern = BOSS_E_IDLE;
		}
		m_pBoss1->ChangeState(A_Pattern);
	}
	if (m_pBoss2->Get_State() == BOSS_IDLE && m_pBoss1->Get_IDLE_END()) {
		B_Pattern = (BOSS_STATE)CFunction::GetInt(3, (int)BOSS_ENHANCE - 1);
		if (B_PrevPattern == BOSS_WAIT && B_Pattern == BOSS_WAIT) {
			B_Pattern = BOSS_E_IDLE;
		}
		m_pBoss2->ChangeState(B_Pattern);
	}

	m_patternTime = 0;
}

void CBossStage::Phase2Pattern()
{
	m_patternTime += CTimeMgr::Get_Instance()->Get_Delta();
	if (m_patternTime < 5.f) { return; }	//3초룰

	BOSS_STATE C_Pattern;
	if (m_pBoss3->Get_State() == BOSS_E_IDLE) {
		C_Pattern = (BOSS_STATE)CFunction::GetInt(11, (int)BOSS_NONE - 1);
		m_pBoss3->ChangeState(C_Pattern);
	}
	m_patternTime = 0;
}

void CBossStage::Check_Boss_Hp()
{
	if (!m_b2phase)
	{
		// 빈사 상태 시 WAIT 전환
		CheckAndEnterWaitState(m_pBoss1);
		CheckAndEnterWaitState(m_pBoss2);

		//사망 → 강화 전환
		if (HandleBossDeathTransition(m_pBoss1, m_pBoss2)) return;
		if (HandleBossDeathTransition(m_pBoss2, m_pBoss1)) return;
	}
	else
	{
		if (m_pBoss3 && m_pBoss3->Get_Dead() && !m_bBoss3DeadHandled)
		{
			m_pBoss3->ChangeState(BOSS_E_DEAD);
			m_bBoss3DeadHandled = true;
			m_bBossDead = true;
		}
	}
}
void CBossStage::CheckAndEnterWaitState(CBoss* boss)
{
	if (boss && boss->ReturnHpRatio() < 0.2f && !boss->IsInState(BOSS_WAIT))
	{
		//boss->ChangeState(BOSS_WAIT);
	}
}
bool CBossStage::HandleBossDeathTransition(CBoss* deadBoss, CBoss* survivingBoss)
{
	if (deadBoss && deadBoss->Get_Dead())
	{
		deadBoss->ChangeState(BOSS_DEAD1);
		survivingBoss->ChangeState(BOSS_ENHANCE);
		m_pBackBoss->SetAnim(BOSS_DEAD1);

		m_pBoss3 = survivingBoss;
		m_b2phase = true;
		m_bBoss3DeadHandled = false;

		m_pUI->ChangePhase();
		return true;
	}
	return false;
}

void CBossStage::Stage_End()
{
	if (!m_pBoss3->Is_Activate()) {

	}
}
