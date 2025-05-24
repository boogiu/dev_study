#include "pch.h"
#include "CGiantSoldier.h"
#include "CRader.h"
#include "CTimeMgr.h"
#include "CResourceMgr.h"
#include "CPlayerRader.h"

CGiantSoldier::CGiantSoldier()
	:m_AttakDelay(3.f), m_ElapsedTime(0)
{
}

CGiantSoldier::~CGiantSoldier()
{
	Release();
}

void CGiantSoldier::Initialize()
{
	m_MonsterName = L"Giant_Soldier";
	INFO info{ 600,1500,400,300 };
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
	m_pCollider->Get_Info().Copy(m_tInfo);
	m_pCollider->Get_Info().Add_OffsetX(-75.f, -50.f);
	m_pCollider->Get_Info().Add_OffsetY(-120.f, -10.f);
	if (!m_pRader) {
		m_pRader = new CRader(this);
	}
	m_pRader->Initialize();
	m_pRader->Get_Info().Copy(m_pCollider->Get_Info());
	m_pRader->Get_Info().Add_OffsetX(350.f, 350.f);

	m_StateContainer[MONSTER_IDLE] = new CGiantIdle;
	m_StateContainer[MONSTER_MOVE] = new CGiantMoveState;
	m_StateContainer[MONSTER_ATTACKA] = new CGiantAttackA;
	m_StateContainer[MONSTER_ATTACKB] = new CGiantAttackB;
	m_StateContainer[MONSTER_DEAD] = new CMonsterDead;
	m_StateContainer[MONSTER_APPEAR] = new CMonsterAppear;

	CMonster::ChangeState(MONSTER_APPEAR);
}

void CGiantSoldier::Update()
{
	m_tInfo.Update_INFO(m_tRect);

	CMonster::Anim_Direction();
	m_StateContainer[m_nowState]->Update(this);

	m_tInfo.fX += m_tVector.fX * CTimeMgr::Get_Instance()->Get_Delta() * m_fMoveSpeed;
	m_tInfo.fY += m_tVector.fY * CTimeMgr::Get_Instance()->Get_Delta() * 0.9f;

	m_ElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
	if (m_ElapsedTime > m_AttakDelay) {
		m_bAttack = true;
	}
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
	}
}

void CGiantSoldier::Late_Update()
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

void CGiantSoldier::Render(HDC _hDC)
{
	m_tInfo.Update_INFO(m_tRect);
	if (CCamera::Get_Instance()->Out_of_Camera(m_tRect)) return;
	if (m_pAnim)
		m_pAnim->Render(_hDC);
}

void CGiantSoldier::Release()
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

void CGiantSoldier::OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType)
{
	if (dynamic_cast<CPlayerRader*>(object)) { return; }

	if (_hitType == HITTED) {
		CMonster::Get_Damage(10);
		CResourceMgr::Get_Instance()->PlaySoundW(L"Whip_Hit", 0.14f);
	}
	else {
		object->Add_Velocity(m_Dir * 10.f, -20.f);
		CResourceMgr::Get_Instance()->PlaySoundW(L"Hit_Blunt_Large", 0.14f);
	}
}

void CGiantSoldier::OnCollisionWall(int dir)
{
	if (!m_bWall) {
		m_Dir *= -1;
		m_tInfo.fX += m_Dir;
		m_bWall = true;
	}
}

void CGiantSoldier::Attacked()
{
	m_ElapsedTime = 0;
	m_bAttack = false;
}

void CGiantSoldier::Find_Target(CObject* target)
{
	if (m_bTargeting) { return; }
	m_bTargeting = true;
	m_pTarget = target;
}

void CGiantSoldier::Not_Find_Target()
{
	if (m_nowState == MONSTER_APPEAR) return;
	
	m_bTargeting = false;
	m_pTarget = nullptr;
}
