#include "pch.h"
#include "CMonster.h"
#include "CBullet.h"
#include"CPhysicsMgr.h"

CMonster::CMonster()
	:m_bChaseMod(false), m_target(nullptr),m_OnHitTime(GetTickCount64())
{
	m_sState = sIDLE;
}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	m_tInfo.fCX = 50;
	m_tInfo.fCY = 50;

	m_tInfo.fX = CRandom::GetRandom(0, WINCX);
	m_tInfo.fY = CRandom::GetRandom(0, WINCY);
	m_fSpeed = CRandom::GetRandom(1, 3);

	CPhysicsMgr::GetInstance().RegisterPhysics(this, OBJ_LAYER::ENEMY, PHYSICS::VELOCITY);
	CPhysicsMgr::GetInstance().RegisterPhysics(this, OBJ_LAYER::ENEMY, PHYSICS::COLLIDER);
}

void CMonster::Update()
{
	CObject::Update_Rect();
	ChaseTarget();
	MoveMonster();
	Update_State();
}

void CMonster::Render(HDC _hDC)
{
	Ellipse(_hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	TCHAR tmpBuffer[32];
	swprintf_s(tmpBuffer, L"%1.f , %1.f", m_tVelocity.fVX, m_tVelocity.fVY);
	DrawText(_hDC, tmpBuffer, lstrlen(tmpBuffer), &m_tRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CMonster::Release()
{
}

void CMonster::Activate()
{
	m_sState = sMOVE;
}

void CMonster::DeActivte()
{

}

void CMonster::OnCollisionEnter(CObject* _object)
{
	if (dynamic_cast<CBullet*>(_object)) {
		m_tVelocity.fVX = _object->GetVelocity().fVX;
		m_tVelocity.fVY = _object->GetVelocity().fVY;
		OnHit();
	}
}


void CMonster::SetTarget(CObject* _target)
{
		m_target = _target;
}

void CMonster::ChaseTarget()
{
	if (!m_target) {
		return;
	}

	if (!(m_sState & sMOVE)) {
		return;
	}
	INFO& targetInfo = m_target->GetPos();

	float xDistance = targetInfo.fX - m_tInfo.fX;
	float yDistance = targetInfo.fY - m_tInfo.fY;

	float fLean = sqrtf(xDistance * xDistance + yDistance * yDistance); //피타고라스 정리 (빗변 길이)

	if (fLean > 0.0f)
	{
		m_tVelocity.fVX = (xDistance / fLean);
		m_tVelocity.fVY = (yDistance / fLean);
	}
}

void CMonster::MoveMonster()
{
	m_tInfo.fX += (m_tVelocity.fVX * m_fSpeed);
	m_tInfo.fY += (m_tVelocity.fVY * m_fSpeed);
}

void CMonster::OnHit()
{
	if (!(m_sState & sONHIT)) {
		m_OnHitTime = GetTickCount64();
		m_sState |= sONHIT; 
		m_sState &= ~(sMOVE);
	}
}

void CMonster::Update_State()
{

	if (m_sState & sONHIT) {
		if (m_OnHitTime + 100 < GetTickCount64()) {
			m_sState &= (~sONHIT);
			m_sState |= sMOVE;
		}
	}
}
