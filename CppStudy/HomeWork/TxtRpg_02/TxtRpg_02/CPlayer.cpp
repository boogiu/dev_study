#include "pch.h"
#include "CPlayer.h"

CPlayer::CPlayer()
	: m_cName(nullptr), m_Class(NOCLASS), m_iLevel(1), m_iMaxExp(10)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Init()
{
	m_Stat.m_iMaxHp = 100;
	m_Stat.m_iNowHp = m_Stat.m_iMaxHp;

	m_Stat.m_iAtk = 5;
	m_Stat.m_iDef = 5;

	m_Stat.m_iCrit = 15;
	m_Stat.m_fCritDgm = 20;

	m_Stat.m_iAvd = 5;

	m_Stat.m_iExp = 0;
	m_Stat.m_iGold = 100;
}

void CPlayer::Release()
{
	SAFE_DELETE_ARR(m_cName);
}

void CPlayer::SetClass(E_CLASS _class)
{
	switch (_class)
	{
	case NOCLASS:
		break;
	case WARRIOR:
		m_Stat.m_iMaxHp = 150;
		m_Stat.m_iNowHp = m_Stat.m_iMaxHp;
		m_Stat.m_iDef = 10;
		break;
	case MAGE:
		m_Stat.m_iAtk = 10;
		m_Stat.m_iCrit = 20;
		break;
	case ROGUE:
		m_Stat.m_iCrit = 30;
		m_Stat.m_iAvd = 15;
		break;
	case ARCHOR:
		m_Stat.m_iCrit = 30;
		m_Stat.m_fCritDgm = 70;
		break;
	default:
		break;
	}
}

void CPlayer::ShowStatus()
{
	
}

void CPlayer::LevelUp()
{
	std::cout << "플레이어 레벨 업!" << std::endl;
	m_iLevel += 1;

	m_Stat.m_iMaxHp += 10;
	m_Stat.m_iNowHp = m_Stat.m_iMaxHp;
	m_Stat.m_iDef += 5;
	m_Stat.m_iAtk += 5;
	m_Stat.m_iCrit += 5;
	m_Stat.m_iAvd += 1;
	m_Stat.m_iCrit += 5;
	m_Stat.m_fCritDgm += 3;

	if (m_Stat.m_iCrit > 100) {
		m_Stat.m_iCrit = 100;
		int tmp = m_Stat.m_iCrit - 100;
		m_Stat.m_iCrit += tmp;
	}

	if (m_Stat.m_iAvd > 100) {
		m_Stat.m_iAvd = 100;
	}
}

void CPlayer::Purchase()
{
}

void CPlayer::GetBooty(CObject* object)
{
	int tmp = object->GetExp() + m_Stat.m_iExp;
	while (tmp >= m_iMaxExp) {
		LevelUp();
		tmp -= m_iMaxExp;
	}
	m_Stat.m_iExp += object->GetExp();
	m_Stat.m_iGold += object->GetGold();
}

void CPlayer::SetName()
{
	m_cName = CFunction::SafeChar();
}
