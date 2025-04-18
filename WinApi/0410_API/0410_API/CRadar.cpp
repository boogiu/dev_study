#include "pch.h"
#include "CRadar.h"
#include "CPhysicsMgr.h"
#include "CMonster.h"

CRadar::CRadar()
{
}

CRadar::~CRadar()
{
	Release();
}

void CRadar::Initialize()
{
	m_tInfo.fCX = 580;
	m_tInfo.fCY = 580;
	CPhysicsMgr::GetInstance().RegisterPhysics(this, OBJ_LAYER::PLAYER, PHYSICS::COLLIDER);
}

void CRadar::Update()
{
	CObject::Update_Rect();

	if (!m_vecTargets.empty()) {
		if (m_vecTargets.front()->GetActivate()) {
			m_vecTargets.pop();
		}
	}
}

void CRadar::Render(HDC _hDC)
{
	Rectangle(_hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CRadar::Release()
{
}

void CRadar::Activate()
{
}

void CRadar::DeActivte()
{
}

CObject* CRadar::GetTarget()
{
	if (m_vecTargets.empty()) {
		return nullptr;
	}
	else
	{
		return m_vecTargets.front();
	}
}

void CRadar::OnCollisionEnter(CObject* _object)
{
	if (dynamic_cast<CMonster*>(_object)) {
		m_vecTargets.push(_object);
	}
}

void CRadar::OnCollisionOut(CObject* _object)
{
	if (!m_vecTargets.empty()) {
		if (_object == m_vecTargets.front()) {
			m_vecTargets.pop();
		}
	}
}
