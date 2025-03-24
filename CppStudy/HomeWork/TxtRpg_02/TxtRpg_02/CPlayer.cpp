#include "pch.h"
#include "CPlayer.h"
#include "CInventory.h"
#include "CItem.h"

CPlayer::CPlayer()
	: m_cName(nullptr), m_Class(NOCLASS), m_iLevel(1), m_iMaxExp(10),m_inven(nullptr)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Init()
{
	//고쳐야 함 : 데이터베이스로 가도록
	m_Stat.m_iMaxHp = 100;
	m_Stat.m_iNowHp = m_Stat.m_iMaxHp;

	m_Stat.m_iAtk = 5;
	m_Stat.m_iDef = 5;

	m_Stat.m_iCrit = 15;
	m_Stat.m_fCritDgm = 20;

	m_Stat.m_iAvd = 5;

	m_Stat.m_iExp = 0;
	m_Stat.m_iGold = 100;

	//-------------------//
	if (!m_inven) {
		m_inven = new CInventory;
	}
}

void CPlayer::Release()
{
	SAFE_DELETE_ARR(m_cName);
	m_inven->Release();
	SAFE_DELETE(m_inven);
}

void CPlayer::SetClass(E_CLASS _class)
{
	m_Class = _class;
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
	while (true)
	{
		std::cout << m_cName << "[" << CFunction::ReturnClass(m_Class) << "]" << std::endl;
		std::cout << "플레이어 레벨 : " << m_iLevel << std::endl;
		std::cout << "소지금 : " << m_Stat.m_iGold << std::endl;
		std::cout << "체력 : " << m_Stat.m_iNowHp << "/" << m_Stat.m_iMaxHp << std::endl;
		std::cout << "방어력 : " << m_Stat.m_iDef << std::endl;
		std::cout << "크리티컬 : " << m_Stat.m_iCrit << "%\t크리티컬 데미지: " << m_Stat.m_fCritDgm + 100 << "%" << std::endl;
		std::cout << "회피 : " << m_Stat.m_iAvd << std::endl;

		std::cout << std::endl;
		std::cout << "1. 인벤토리 \t 2. 돌아가기 "  << std::endl;

		int iSelect = CFunction::SafeInt(1, 2);
		if (iSelect == 2) {
			return;
		}
		m_inven->Render();
	}
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
	m_iMaxExp += 10;
}

bool CPlayer::Purchase(CItem* _item)
{
	if (m_Stat.m_iGold < _item->GetCostl()) {
		return false;
	}
	if (m_inven->PushItem(_item)) {
		m_Stat.m_iGold -= _item->GetCostl();
	}
	return true;
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
