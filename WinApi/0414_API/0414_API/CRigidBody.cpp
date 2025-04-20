#include "pch.h"
#include "CRigidBody.h"
#include "CGameObject.h"
#include "CTransform.h"
CRigidBody::CRigidBody()
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::Initialize()
{
}

void CRigidBody::Update()
{
	VECTOR2& transform = dynamic_cast<CTransform*>(m_pOwner->GetComponent<CTransform>())->GetPos();
	transform += m_vVelocity;
	
	//¸¶Âû·Â
	m_vVelocity.fX *= 0.5f;

	if (m_bGravity) {
		m_vVelocity.fY += 0.5f;
	}
}

void CRigidBody::Late_Update()
{

}

void CRigidBody::Render(HDC _hDC)
{
}

void CRigidBody::Release()
{
}

void CRigidBody::ImPulse(VECTOR2 _velocity)
{
	m_vVelocity += _velocity;
}
