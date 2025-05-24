#include "pch.h"
#include "CPlayerRader.h"
#include "CAttackBox.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CSkul.h"

CPlayerRader::CPlayerRader(CPlayer* owner)
	:m_pTarget(nullptr)
{
	m_pOwner = owner;
}

CPlayerRader::~CPlayerRader()
{
	Release();
}

void CPlayerRader::Initialize()
{
	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox(this);
		m_pAtkBox->Set_ColType(CONSTANT);
		m_pAtkBox->Set_Target(MONSTER);
		m_pAtkBox->Set_Counter(4);
	}
	m_pAtkBox->Set_Active();
}

void CPlayerRader::Update()
{
	m_tInfo.fX = m_pOwner->Get_Info().fX;
	m_tInfo.fY = m_pOwner->Get_Info().fY;
	m_pAtkBox->Get_Info().Copy(m_tInfo);

	if (m_pTarget) {
		if (CMonster* monster = dynamic_cast<CMonster*>(m_pTarget)) {
		}
	}
	else {
		m_pOwner->Get_Skul()->Not_Find_Target();
	}
}

void CPlayerRader::Late_Update()
{
	m_pTarget = nullptr;
	if (m_pOwner) {
		if (!m_pOwner->Is_Activate()) {
			m_bActive = false;
			m_pAtkBox->Set_DeActive();
		}
	}
}

void CPlayerRader::Render(HDC _hDC)
{
}

void CPlayerRader::Release()
{
	m_pAtkBox->Set_DeActive();
	Safe_Delete<CAttackBox*>(m_pAtkBox);
}

void CPlayerRader::OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType)
{

	if (CMonster* monster =dynamic_cast<CMonster*>(object)) {
		m_pOwner->Get_Skul()->Find_Target(monster);
		m_pTarget = object;
	}
}

void CPlayerRader::Set_Active(bool _active)
{
	if (_active == false) {
		m_pAtkBox->Set_DeActive();
	}
	m_bActive = _active;
}
