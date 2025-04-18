#include "pch.h"
#include "CVelocity.h"
#include "CObject.h"
#include "CObjMgr.h"
#include "CPhysicsMgr.h"

CVelocity::CVelocity()
	: m_fFriction(0.f),m_fResistance(0.f), m_pMovableObj(nullptr)
{
}

CVelocity::~CVelocity()
{
	Relese();
}

void CVelocity::Initialize()
{
	m_fFriction = 0.25f;
	m_fResistance = 0.04f;
}

void CVelocity::Update()
{
	if (!m_pMovableObj) {
		m_pMovableObj=CPhysicsMgr::GetInstance().GetPhysicsObject(PHYSICS::VELOCITY);
	}
	if (m_pMovableObj->empty()) {
		return;
	}

	for_each(m_pMovableObj->begin(), m_pMovableObj->end(),
		[this](CObject* pObj) {
			if (!pObj->GetActivate()) return;

			if (pObj->GetState() & sMOVE) {
				pObj->GetVelocity().fVX *= this->GetFriction();
			}

			if (abs(pObj->GetVelocity().fVX) < 0.01f) {
				pObj->GetVelocity().fVX = 0;
			}
		});
}
void CVelocity::Render()
{
}

void CVelocity::Relese()
{
}
