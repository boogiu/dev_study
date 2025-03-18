#include "CPlayer.h"
#include "pch.h"

CPlayer::CPlayer()
	:m_cName(nullptr), m_Class(CLASS_ENUM::NONE), m_iGold(0)
{
	memset(&m_pInfo, 0, sizeof(m_pInfo));
}

CPlayer::~CPlayer()
{
}

void CPlayer::Init()
{
	m_pInfo.m_iAtk = 15;
	m_pInfo.m_iMaxHp = 200;
	m_pInfo.m_iNowHp = m_pInfo.m_iMaxHp;
}

void CPlayer::Render()
{
}

void CPlayer::Release()
{
	if (!m_cName) return;
	SAFE_DELETE_ARR(m_cName);
}

void CPlayer::SetClass(CLASS_ENUM _class)
{
	switch (_class)
	{
	case NONE:
		break;
	case WARRIOR:
		cout << "전사 : " << endl;
		break;
	case MAGE:
		cout << "전사 : " << endl;
		break;
	case ROGUE:
		cout << "전사 : " << endl;
		break;
	case ARCHOR:
		cout << "전사 : " << endl;
		break;
	default:
		break;
	}
	system("pause");
}

void CPlayer::SetName()
{
	m_cName = SafeChar();
}

char* CPlayer::GetName()
{
	return m_cName;
}

void CPlayer::ShowStat()
{
	cout << m_cName<< endl;
	cout << "체력 : " << m_pInfo.m_iNowHp <<"/"<< m_pInfo.m_iMaxHp << endl;
	cout << "공격력 : " << m_pInfo.m_iAtk << endl;
	cout << m_cName << endl;
}

void CPlayer::GetDamage(int dmg)
{
	cout << m_cName << "은 " << dmg << "만큼의 피해를 입었다!" << endl;
	if (m_pInfo.m_iNowHp - dmg <= 0) {
		m_pInfo.m_iNowHp = 0;
	}
	else {
		m_pInfo.m_iNowHp -= dmg;
	}
}

void CPlayer::Restore()
{
	m_pInfo.m_iNowHp = m_pInfo.m_iMaxHp;
}
