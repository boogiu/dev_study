#include "pch.h"
#include "CRader.h"
#include "CAttackBox.h"
#include "CPlayer.h"
#include "CMonster.h"

CRader::CRader(CMonster* owner)
	:m_pTarget(nullptr)
{
	m_pOwner = owner;
}

CRader::~CRader()
{
	Release();
}

void CRader::Initialize()
{
	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(this);
		m_pAtkBox->Set_ColType(CONSTANT);
	}
	m_pAtkBox->Set_Active();
}

void CRader::Update()
{
	m_tInfo.fX = m_pOwner->Get_Info().fX;
	m_tInfo.fY = m_pOwner->Get_Info().fY;
	m_pAtkBox->Get_Info().Copy(m_tInfo);

	if (m_pTarget) {
		//m_pOwner->Find_Target(m_pTarget);
	}
	else {
		m_pOwner->Not_Find_Target();
	}
}

void CRader::Late_Update()
{
	m_pTarget = nullptr;
	if (m_pOwner) {
		if (!m_pOwner->Is_Activate()) {
			m_bActive = false;
			m_pAtkBox->Set_DeActive();
		}
	}
}

void CRader::Render(HDC _hDC)
{
}

void CRader::Release()
{
	m_pAtkBox->Set_DeActive();
	Safe_Delete<CAttackBox*>(m_pAtkBox);
}

void CRader::OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType)
{
	if (m_pTarget) return;

	if (dynamic_cast<CPlayer*>(object)) {
		m_pOwner->Find_Target(object);
		m_pTarget = object;
	}
}

void CRader::Set_Active(bool _active)
{
	if (_active == false) {
		m_pAtkBox->Set_DeActive();
	}
	m_bActive=_active;
}
