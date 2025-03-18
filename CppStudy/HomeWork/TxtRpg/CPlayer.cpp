#include "CPlayer.h"
#include "pch.h"

CPlayer::CPlayer()
	:m_cName(nullptr),m_iGold(0),m_iMaxHp(0),m_iNowHp(0),m_iAtk(0)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
	m_iGold = 100;
	m_iAtk = 10;
	m_iMaxHp = 100;
	m_iNowHp = 100;
}

void CPlayer::Update()
{

}

void CPlayer::Render()
{
}

void CPlayer::Release()
{
	delete[] m_cName;
}

void CPlayer::SetName()
{
	if (m_cName != nullptr) return;
	m_cName = SafeChar();
}

char*  CPlayer::GetName()
{
	if (m_cName == nullptr) return nullptr;
	return m_cName;
}

void CPlayer::SetClass(int _class)
{
	switch (_class)
	{
	case 1:
		m_iGold = 100;
		m_iAtk = 10;
		m_iMaxHp = 100;
		m_iNowHp = m_iMaxHp;
		break;
	case 2:
		m_iGold = 100;
		m_iAtk = 10;
		m_iMaxHp = 100;
		m_iNowHp = m_iMaxHp;
		break;
	case 3:
		m_iGold = 100;
		m_iAtk = 10;
		m_iMaxHp = 100;
		m_iNowHp = m_iMaxHp;
		break;
	case 4:
		m_iGold = 100;
		m_iAtk = 10;
		m_iMaxHp = 100;
		m_iNowHp = m_iMaxHp;
		break;
	default:
		break;
	}
}

int CPlayer::GetGold()
{
	return m_iGold;
}

void CPlayer::GetDamage(int Dmg)
{
	m_iNowHp -= Dmg;
}

void CPlayer::Restore()
{
}
