#include "pch.h"
#include "CRigidBody.h"
#include "CObject.h"
#include "CTransform.h"

CRigidBody::CRigidBody()
	:m_bFriction(false), m_fFriction(1.f)
{
	m_vVelocity = { 0.f,0.f,0.f };
	m_vAngular = { 0.f,0.f,0.f };
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::Initialize()
{
}

void CRigidBody::Update(float dt)
{
	if (CTransform* transform = m_pOwner->GetComponent<CTransform>()) {
		transform->Add_Position(m_vVelocity * dt);
		transform->Add_Rotate(m_vAngular * dt);
	}
}

void CRigidBody::Late_Update(float dt)
{
	if (m_bFriction) {
		m_vVelocity*= m_fFriction;
		m_vAngular *= m_fFriction;
	}
}

void CRigidBody::Impulse(float _x, float _y, float _z)
{
	m_vVelocity.x += _x;
	m_vVelocity.y += _y;
	m_vVelocity.z += _z;
}

void CRigidBody::ImpulseRotate(float _x, float _y, float _z)
{
	m_vAngular.x += _x;
	m_vAngular.y += _y;
	m_vAngular.z += _z;
}

void CRigidBody::Set_Friction(bool set, float friction)
{
	m_bFriction = set;
	m_fFriction = friction;
}
