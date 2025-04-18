#include "pch.h"
#include "CBullet.h"
#include "CPhysicsMgr.h"
#include "Objects.h"
#include "CBorder.h"

CBullet::CBullet()
{
}

CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
	m_tInfo.fX = 0;
	m_tInfo.fY = 0;
	m_tInfo.fCX = 10;
	m_tInfo.fCY = 10;


	m_fSpeed = 8.f;

	CPhysicsMgr::GetInstance().RegisterPhysics(this, OBJ_LAYER::BULLET, PHYSICS::VELOCITY);
	CPhysicsMgr::GetInstance().RegisterPhysics(this, OBJ_LAYER::BULLET, PHYSICS::COLLIDER);

}

void CBullet::Update()
{
	CObject::Update_Rect();

	MoveBullet();
}

void CBullet::Render(HDC _hDC)
{
	Ellipse(_hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CBullet::Release()
{

}


void CBullet::Activate()
{
	m_tVelocity.fVX = 0.f;
	m_tVelocity.fVY = -1.f;
	m_bActive = true;
}

void CBullet::DeActivte()
{
	m_bActive = false;
}

void CBullet::SetTarget(INFO target)
{
	float xDistance = target.fX - m_tInfo.fX;
	float yDistance = target.fY - m_tInfo.fY;

	float fDistance = sqrtf(xDistance * xDistance + yDistance * yDistance);
	if (fDistance > 0.0f)
	{
		m_tVelocity.fVX = (xDistance / fDistance);
		m_tVelocity.fVY = (yDistance / fDistance);
	}
}

void CBullet::OnCollisionEnter(CObject* _obj)
{
	if (dynamic_cast<CMonster*>(_obj)) {
		DeActivte();
	}
}

void CBullet::OnCollisionEnter(CTile* _tile)
{
	if (dynamic_cast<CBorder*>(_tile)) {
		m_bOut = false;
	}
}

void CBullet::OnCollisionOut(CTile* _tile)
{
	if (dynamic_cast<CBorder*>(_tile)) {
		DeActivte();
		m_bOut = true;
	}
}

void CBullet::MoveBullet()
{
	m_tInfo.fX += m_tVelocity.fVX * m_fSpeed;
	m_tInfo.fY += m_tVelocity.fVY * m_fSpeed;
}

