#include "pch.h"
#include "CSoldier.h"
#include "MonsterStates.h"
#include "CTimeMgr.h"
#include "CRader.h"
#include "CResourceMgr.h"
#include "CPlayerRader.h"

CSoldier::CSoldier()
	:m_AttakDelay(3.f),m_ElapsedTime(0)
{
}

CSoldier::~CSoldier()
{
	Release();
}

void CSoldier::Initialize()
{
	m_MonsterName = L"Soldier";
	INFO info{ 600,1500,200,200 };
	m_tInfo.Translate_Info(info);
	m_fMoveSpeed = 15.f;
	m_MaxHp = 15;
	m_NowHp = m_MaxHp;
	if (!m_pAnim) {
		m_pAnim = new CAnim(this);
	}
	if (!m_pCollider) {
		m_pCollider = new CCollider(this);
	}
	if (!m_pRader) {
		m_pRader = new CRader(this);
		m_pRader->Initialize();
		m_pRader->Get_Info().Copy(m_tInfo);
		m_pRader->Get_Info().Add_OffsetX(100.f, 100.f);
	}

	m_pCollider->Get_Info().Copy(m_tInfo);
	m_pCollider->Get_Info().Add_OffsetX(-75.f, -50.f);
	m_pCollider->Get_Info().Add_OffsetY(-80.f, -10.f);
	m_StateContainer[MONSTER_IDLE] = new CSoldierIdle;
	m_StateContainer[MONSTER_MOVE] = new CSoldierMove;
	m_StateContainer[MONSTER_ATTACKA] = new CSoldierAttack;
	m_StateContainer[MONSTER_HIT] = new CSoldierHit;
	m_StateContainer[MONSTER_DEAD] = new CMonsterDead;
	m_StateContainer[MONSTER_APPEAR] = new CMonsterAppear;

	CMonster::ChangeState(MONSTER_APPEAR);
}

void CSoldier::Update()
{

	CMonster::Anim_Direction();
	m_StateContainer[m_nowState]->Update(this);

	m_tInfo.fX += m_tVector.fX * CTimeMgr::Get_Instance()->Get_Delta() * m_fMoveSpeed;
	m_tInfo.fY += m_tVector.fY * CTimeMgr::Get_Instance()->Get_Delta() * 0.9f;

	m_ElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();

	if (m_pAnim)
		m_pAnim->Update();

	if (m_pCollider) {
		m_pCollider->Update();
		m_pCollider->Update_Center(m_tInfo.fX, m_tInfo.fY);
	}
	if (m_pRader) {
		m_pRader->Update();
	}

	if (m_pTarget) {
		float distance = fabs(m_pTarget->Get_Info().fX - m_tInfo.fX);

		if (m_pTarget->Get_Info().fX > m_tInfo.fX) {
			m_Dir = 1;
		}
		else {
			m_Dir = -1;
		}
		if (distance < 80 && m_ElapsedTime > m_AttakDelay) {
			CMonster::ChangeState(MONSTER_ATTACKA);
			m_ElapsedTime = 0;
		}
	}
}

void CSoldier::Late_Update()
{

	float friction = 0.8f;
	m_tVector.fX *= friction;
	if (fabs(m_tVector.fX) < 0.1f) m_tVector.fX = 0.f;

	if (m_pAnim)
		m_pAnim->Late_Update();
	if (m_pCollider)
		m_pCollider->Late_Update();
	if (m_pRader) {
		m_pRader->Late_Update();
	}

	if (!m_bGround) {
		Add_Velocity(0.f, GRAVITY * 8);
	}
	if (fabs(m_tVector.fY) < 0.1f) {
		m_tVector.fY = 0.f;
	}
}

void CSoldier::Render(HDC _hDC)
{
	m_tInfo.Update_INFO(m_tRect);
	if (CCamera::Get_Instance()->Out_of_Camera(m_tRect)) return;
	if (m_pAnim)
		m_pAnim->Render(_hDC);
}

void CSoldier::Release()
{
	Safe_Delete<CAnim*>(m_pAnim);
	Safe_Delete<CCollider*>(m_pCollider);
	Safe_Delete<CRader*>(m_pRader);
	if (!m_StateContainer.empty()) {
		auto iter = m_StateContainer.begin();
		for (;iter != m_StateContainer.end(); ++iter) {
			Safe_Delete<CMonsterState*>(iter->second);
		}
	}
	m_StateContainer.clear();
}

void CSoldier::OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType)
{
	if (dynamic_cast<CPlayerRader*>(object)) { return; }

	if (_hitType == HITTED) {
		CMonster::ChangeState(MONSTER_HIT);
		CMonster::Get_Damage(10);
	}
	else {
		CResourceMgr::Get_Instance()->PlaySoundW(L"hit_blade", 0.14f);
	}
}

void CSoldier::Find_Target(CObject* target)
{
	if (m_bTargeting) { return; }
	m_bTargeting = true;
	m_pTarget = target;
	
	CMonster::ChangeState(MONSTER_MOVE);
}

void CSoldier::Not_Find_Target()
{
	if (m_nowState == MONSTER_APPEAR) return;
	CMonster::ChangeState(MONSTER_IDLE);
 	m_bTargeting = false;
	m_pTarget = nullptr;
}