#include "pch.h"
#include "CGravity.h"
#include "CObject.h"
#include "CPhysicsMgr.h"

CGravity::CGravity()
	: m_fGravity(0),m_fMaxGravity(0), m_pGravObj(nullptr)
{
}

CGravity::~CGravity()
{
	Release();
}

void CGravity::Initialize()
{
	m_fGravity = 0.098f;
	m_fMaxGravity = 1.5f;
}

void CGravity::Update()
{
	if (!m_pGravObj) {
		m_pGravObj = CPhysicsMgr::GetInstance().GetPhysicsObject(PHYSICS::GRAVITY);
	}

	if (m_pGravObj->empty()) {
		return;
	}

	for_each(m_pGravObj->begin(), m_pGravObj->end(),
		[this](CObject* pObj) {
			if (!pObj->GetActivate()) return;

			if (abs(pObj->GetVelocity().fVY) < 0.4f) {
				pObj->GetVelocity().fVY += (m_fGravity*0.35f);
			}

			else {
				pObj->GetVelocity().fVY += m_fGravity;
			}
			if (pObj->GetVelocity().fVY >= m_fMaxGravity) {
				pObj->GetVelocity().fVY = m_fMaxGravity;
			}
		});
}

void CGravity::Render()
{
}

void CGravity::Release()
{
}

void CGravity::GravityUpdate()
{
}
