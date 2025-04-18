#include "pch.h"
#include "CBullet.h"
#include "Components.h"

CBullet::CBullet(VECTOR2 _pos)
	: m_fRadian(0.f), m_fRadius(2.f)
{
	AddComponent(new CTransform);
	AddComponent(new CRigidBody);
	m_Center = _pos;
}
CBullet::~CBullet()
{
}
void CBullet::Initialize()
{
	GetComponent<CRigidBody>()->SetVelocity({ 1.f,0.f });
	GetComponent<CTransform>()->SetScale({ 10.f,10.f });
	GetComponent<CTransform>()->SetPos(m_Center);

}

void CBullet::Update()
{
	__super::Update_Component();
	m_Center = GetComponent<CTransform>()->GetPos();
	m_fRadian += 0.65f; // or 원하는 속도만큼 증가

	// 원 운동 위치 계산
	VECTOR2 orbitOffset = {
		cosf(m_fRadian) * m_fRadius,
		sinf(m_fRadian) * m_fRadius
	};

	GetComponent<CRigidBody>()->ImPulse({ orbitOffset.fX +1, orbitOffset.fY });
}

void CBullet::Late_Update()
{

}

void CBullet::Render(HDC _hDC)
{
	__super::Render_Component(_hDC);
}

void CBullet::Release()
{
}

void CBullet::CalcOrbit()
{

}
