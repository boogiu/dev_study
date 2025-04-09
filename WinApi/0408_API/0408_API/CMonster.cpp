#include "pch.h"
#include "CMonster.h"

CMonster::CMonster()
	:m_fMoveRange(0.f), m_fNowMoveRange(0), m_Hp(0), m_bDead(false),m_NowDir(sLeftDir)
{
	ZeroMemory(m_szHp, sizeof(m_szHp));
}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	m_tInfo = { 0, 0, 50, 20 };
	m_fMoveRange = 120;
	m_fSpeed = 1;
	m_Hp = 5;
}

void CMonster::Update()
{
	CObj::Update_Rect();

	if (m_fMoveRange <= m_fNowMoveRange) {
		if (m_NowDir & sLeftDir) {
			m_NowDir |= sRightDir;
			m_NowDir &= ~(sLeftDir);
		}
		else if (m_NowDir & sRightDir) {
			m_NowDir |= sLeftDir;
			m_NowDir &= ~(sRightDir);
		}

		m_fNowMoveRange = 0;
	}

	MonsterMove();
}

void CMonster::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	swprintf_s(m_szHp, L"HP : %d", m_Hp, m_tInfo.fX, m_tInfo.fY);
	DrawText(hDC, m_szHp, lstrlen(m_szHp), &m_tRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CMonster::MonsterMove()
{
	if (m_NowDir & sLeftDir) {
		m_tInfo.fX -= m_fSpeed;
	}
	if (m_NowDir & sRightDir) {
		m_tInfo.fX += m_fSpeed;
	}

	m_fNowMoveRange += m_fSpeed;
}

void CMonster::GetHit()
{
	m_Hp -= 1;
	if (m_Hp <= 0) {
		m_bDead = true;
	}
}

bool CMonster::IsDead()
{
	return m_bDead;
}

void CMonster::Release()
{
}

