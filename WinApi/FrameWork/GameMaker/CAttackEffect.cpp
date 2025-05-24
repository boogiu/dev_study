#include "pch.h"
#include "CAttackEffect.h"
#include "CAttackBox.h"

CAttackEffect::CAttackEffect()
{
}

CAttackEffect::~CAttackEffect()
{
}

void CAttackEffect::Initialize()
{
	if (!m_pAnim) {
		m_pAnim = new CAnim(nullptr);
	}
	if (!m_pAtkBox) {
		m_pAtkBox = new CAttackBox;
	}
}

void CAttackEffect::Update()
{
}

void CAttackEffect::Late_Update()
{
}

void CAttackEffect::Render(HDC hDC)
{

}

void CAttackEffect::Release()
{
	if (m_pAnim) {
		Safe_Delete<CAnim*>(m_pAnim);
	}
	if (m_pAtkBox) {
		Safe_Delete<CAttackBox*>(m_pAtkBox);
	}
}
