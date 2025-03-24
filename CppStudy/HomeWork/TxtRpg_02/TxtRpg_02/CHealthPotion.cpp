#include "pch.h"
#include "CHealthPotion.h"
#include "CPlayer.h"


CHealthPotion::CHealthPotion(tagItem _Info, int _Point)
	:m_HealPoint(0)
{
	m_ItemInfo.category = _Info.category;
	m_ItemInfo.m_iLevel = _Info.m_iLevel;
	m_ItemInfo.m_iCost = _Info.m_iCost;
	m_ItemInfo.m_cName = _Info.m_cName;
	m_ItemInfo.m_cDesc = _Info.m_cDesc;
	m_HealPoint = _Point;
}

CHealthPotion::CHealthPotion(const CHealthPotion& rhs)
	:m_HealPoint(0)
{
	m_ItemInfo.m_iLevel = rhs.m_ItemInfo.m_iLevel;
	m_ItemInfo.m_iCost = rhs.m_ItemInfo.m_iCost;
	m_ItemInfo.category = rhs.m_ItemInfo.category;

	int tmpName = strlen(rhs.m_ItemInfo.m_cName);
	int tmpDesc = strlen(rhs.m_ItemInfo.m_cDesc);

	m_ItemInfo.m_cName = new char[tmpName+1];
	m_ItemInfo.m_cDesc = new char[tmpDesc+1];

	strcpy_s(m_ItemInfo.m_cName, tmpName + 1, rhs.m_ItemInfo.m_cName);
	strcpy_s(m_ItemInfo.m_cDesc, tmpDesc + 1, rhs.m_ItemInfo.m_cDesc);

	m_HealPoint = rhs.m_HealPoint;
}

CHealthPotion::~CHealthPotion()
{
}

void CHealthPotion::Init()
{

}

void CHealthPotion::Release()
{
	SAFE_DELETE_ARR(m_ItemInfo.m_cName);
	SAFE_DELETE_ARR(m_ItemInfo.m_cDesc);
}

void CHealthPotion::Activate(CPlayer * _player)
{
	std::cout << m_ItemInfo.m_cName << "을 사용했습니다." << std::endl;
	_player->Heal(m_HealPoint);
}

void CHealthPotion::Render()
{
	std::cout << m_ItemInfo.m_cName << " (" << m_ItemInfo.m_iCost << "원)" << std::endl;
	std::cout << m_ItemInfo.m_cDesc << std::endl;
}

int CHealthPotion::GetLevel()
{
	return m_ItemInfo.m_iLevel;
}

int CHealthPotion::GetCostl()
{
	return m_ItemInfo.m_iCost;;
}

E_Item CHealthPotion::GetCategory()
{
	return m_ItemInfo.category;
}
