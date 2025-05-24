#include "pch.h"
#include "CBoss.h"
#include "CAnim.h"
#include "CResourceMgr.h"
#include"BossStates.h"
#include "CCollider.h"
#include "CEffectMgr.h"
#include "CPlayer.h"
#include "CPlayerRader.h"
#include "CTimeMgr.h"
CBoss::CBoss()
	:m_pAnim(nullptr), m_nowState(BOSS_NONE), m_WaitPoint{}, m_bEndIdle(false), m_bWaiting(false), m_pTarget(nullptr), m_bRender(true),
	m_MaxHp(0.f),m_NowHp(0.f),m_bDead(false)
{

}

CBoss::~CBoss()
{
	Release();
}

void CBoss::Initialize()
{
	INFO info{ 150,150, 200,200 };
	m_tInfo.Translate_Info(info);
	m_pAnim = new CAnim(this);
	m_pCollider = new CCollider(this);
	m_pCollider->Get_Info().Copy(m_tInfo);
	m_pCollider->Get_Info().Add_OffsetX(-50.f, -50.f);

	//------------------------Phase1------------------------------------//
	m_StateContainer.insert({ BOSS_WAIT , new CBossWait});
	m_StateContainer.insert({ BOSS_INTRO , new CBossIntro });
	m_StateContainer.insert({ BOSS_IDLE , new CBossIdle });
	m_StateContainer.insert({ BOSS_GROUND , new CBossGround });
	m_StateContainer.insert({ BOSS_METEOR , new CBossMeteor });
	m_StateContainer.insert({ BOSS_HOMING , new CBossHoming });
	m_StateContainer.insert({ BOSS_SLASH , new CBossSlash });
	m_StateContainer.insert({ BOSS_DEAD1 , new CBossDead });
	//------------------------PhaseChange------------------------------------//
	m_StateContainer.insert({ BOSS_ENHANCE , new CBossEnHance });
	//------------------------Phase2------------------------------------//
	m_StateContainer.insert({ BOSS_E_DEAD , new CEnhancedBossDead });
	m_StateContainer.insert({ BOSS_E_GROUND , new CEnhancedBossGround });
	//m_StateContainer.insert({ BOSS_E_HOMING , new CEnhancedBossHoming });
	m_StateContainer.insert({ BOSS_E_IDLE , new CEnhancedBossIDLE });
	m_StateContainer.insert({ BOSS_E_METEOR , new CEnhancedBossMeteor });
	m_StateContainer.insert({ BOSS_E_RUSH , new CEnhancedBossRush });
	//m_StateContainer.insert({ BOSS_E_SLASH , new CEnhancedBossGround });

	ChangeState(BOSS_WAIT);
	m_bRender = true;

	m_MaxHp = 500;
	m_NowHp = m_MaxHp;

	if (m_StateContainer.count(BOSS_SLASH))
		OutputDebugString(L"✅ BOSS_SLASH 삽입 확인\n");
	else
		OutputDebugString(L"❌ BOSS_SLASH 삽입 실패\n");
}

void CBoss::Update()
{
	__super::Update_RECT();

	if (m_pAnim)
		m_pAnim->Update();
	if(m_StateContainer[m_nowState])
		m_StateContainer[m_nowState]->Update(this);
	
	m_tInfo.fX += m_tVector.fX* m_Dir;

	float ySpeed = m_tVector.fY * CTimeMgr::Get_Instance()->Get_Delta() * 20*0.9 ;
	if (ySpeed >= 25.f) {
		ySpeed = 25.f;
	}
	else if (ySpeed <= -25.f) {
		ySpeed = -25.f;
	}
	m_tInfo.fY += ySpeed;

	if (m_pCollider) {
		//m_pCollider->Update();
		m_pCollider->Update_Center(m_tInfo.fX, m_tInfo.fY);
	}
}

void CBoss::Late_Update()
{
	if (m_pAnim)
		m_pAnim->Late_Update();
}

void CBoss::Render(HDC _hDC)
{
	if (!m_bRender) return;
	if (m_pAnim)
		m_pAnim->Render(_hDC);
	
}

void CBoss::Release()
{
	Safe_Delete<CAnim*>(m_pAnim);
	Safe_Delete<CCollider*>(m_pCollider);

	for (int i = 0; i < (BOSS_NONE); ++i) {
		if (i == (int)BOSS_SLASH) { 
			int ik= 0;
		}
		delete m_StateContainer[(BOSS_STATE)i];
		m_StateContainer[(BOSS_STATE)i] = nullptr;
	}
//	for (auto& iter : m_StateContainer)
//	{
//		if (iter.second != nullptr)
//		{
//			delete iter.second;         
//			iter.second = nullptr;
//		}
//	}
	m_StateContainer.clear();
	//이거 클리어 안하면 메모리 누수가 약간 산발적으로 나. 이거 지금 보스 씬에 들어오지도 않았는데 누수 난 거거든?
}

void CBoss::ChangeState(BOSS_STATE state)
{
	if (m_nowState == state) return;

	if (m_StateContainer[m_nowState]) {
		m_StateContainer[m_nowState]->Exit(this);
	}

	m_nowState = state;

	if (m_StateContainer[m_nowState]) {
		m_StateContainer[m_nowState]->Enter(this);
	}
}

void CBoss::SetAnim(wstring animKey)
{
	if (!m_pAnim) return;

	wstring dir;
	// 방향 추가
	if (m_Dir == -1)
		dir = L"_L";
	else
		dir=L"_R";
	wstring AnimationKey = m_BossName +L"_" + animKey+ dir;
	ANI_FRAME ani = m_StateContainer[m_nowState]->Get_AnimInfo();
	m_pAnim->Set_Anim(AnimationKey.c_str(), ani);
}


void CBoss::Set_BossType(int i)
{
	if (i == 1) {
		m_BossName = L"BossA";
		m_Dir = -1;
	}
	else if (i == 2) {
		m_BossName = L"BossB";
		m_Dir = 1;
	}
}

void CBoss::Change_anim_path(wstring newPath)
{
	wstring dir;
	// 방향 추가
	if (m_Dir == -1)
		dir = L"_L";
	else
		dir = L"_R";
	wstring AnimationKey = m_BossName + L"_" + newPath + dir;

	m_pAnim->ChangeOnlyPath(AnimationKey.c_str());
}

void CBoss::OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType)
{
	if (dynamic_cast<CPlayerRader*>(object)) { return; }

	if (_hitType == HITTED) {
		Get_Damage(100.f);
	}
}

void CBoss::Get_Damage(float Dmg)
{
	m_NowHp -= Dmg;
	if (m_NowHp < 0) {
		m_NowHp = 0;
		m_bDead = true;
	}
}

void CBoss::Find_Target(CObject* target)
{
}

void CBoss::Not_Find_Target()
{
}
