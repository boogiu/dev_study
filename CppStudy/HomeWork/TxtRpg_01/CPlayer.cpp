#include "CPlayer.h"

CPlayer::CPlayer()
	:m_cName(nullptr),m_Class(CLASS_ENUM::NONE),m_iGold(0), m_iMaxHp(0), m_iNowHp(0), m_iAtk(0)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Init()
{
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
