#include "Engine_Define.h"
#include "CRigidBody.h"
#include "CTransform.h"
#include "CGameObject.h"

CRigidBody* CRigidBody::Create(Rigid_State state)
{
	CRigidBody* instance = new CRigidBody;

	if (FAILED(instance->Ready_Component())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	instance->Set_State(state);

	return instance;
}

HRESULT CRigidBody::Ready_Component()
{
	m_vVelocity = { 0,0,0 };
	m_eState = Rigid_State::KINEMATIC;
	m_bGrounded = false;
	m_fFriction = 0.f;
	return S_OK;
}

void CRigidBody::Update_Component(float& dt)
{
	if (m_eState == Rigid_State::DYNAMIC)
	{
		if (!m_bGrounded)
			m_pTransform->Add_Pos({ 0,-GRAVITY * dt,0 });
	}

	m_pTransform->Add_Pos({ m_vVelocity* dt });
}

void CRigidBody::LateUpdate_Component(float& dt)
{
	m_vVelocity *= (1 - m_fFriction);
}

CComponent* CRigidBody::Clone() const
{
	return nullptr;
}

void CRigidBody::Add_Impulse(_vec3 velocity)
{
	m_vVelocity += velocity;
}

void CRigidBody::Set_Transform()
{
	m_pTransform = m_pOwner->Get_Component<CTransform>();

	if (m_pTransform) {
		m_pTransform->AddRef();
	}
}

void CRigidBody::Free()
{
	Safe_Release(m_pTransform);
}
