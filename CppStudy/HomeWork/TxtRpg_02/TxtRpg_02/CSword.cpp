#include "pch.h"
#include "CSword.h"


CSword::CSword(tagItem _Info, int _Point)
	:m_AtkPoint(0)
{
	m_ItemInfo.category = _Info.category;
	m_ItemInfo.m_iLevel = _Info.m_iLevel;
	m_ItemInfo.m_iCost = _Info.m_iCost;
	m_ItemInfo.m_cName = _Info.m_cName;
	m_ItemInfo.m_cDesc = _Info.m_cDesc;
	m_AtkPoint = _Point;
}

CSword::CSword(const CSword& rhs)
	:m_AtkPoint(0)
{
	m_ItemInfo.m_iLevel = rhs.m_ItemInfo.m_iLevel;
	m_ItemInfo.m_iCost = rhs.m_ItemInfo.m_iCost;
	m_ItemInfo.category = rhs.m_ItemInfo.category;

	int tmpName = strlen(rhs.m_ItemInfo.m_cName);
	int tmpDesc = strlen(rhs.m_ItemInfo.m_cDesc);

	m_ItemInfo.m_cName = new char[tmpName + 1];
	m_ItemInfo.m_cDesc = new char[tmpDesc + 1];

	strcpy_s(m_ItemInfo.m_cName, tmpName + 1, rhs.m_ItemInfo.m_cName);
	strcpy_s(m_ItemInfo.m_cDesc, tmpDesc + 1, rhs.m_ItemInfo.m_cDesc);

	m_AtkPoint = rhs.m_AtkPoint;
}

CSword::~CSword()
{
	
}

void CSword::Init()
{
}

void CSword::Release()
{
	SAFE_DELETE_ARR(m_ItemInfo.m_cName);
	SAFE_DELETE_ARR(m_ItemInfo.m_cDesc);
}

void CSword::Render()
{
		std::cout << m_ItemInfo.m_cName << " (" << m_ItemInfo.m_iCost << "¿ø)" << std::endl;
		std::cout << m_ItemInfo.m_cDesc << std::endl;
}

int CSword::GetLevel()
{
	return m_ItemInfo.m_iLevel;
}

int CSword::GetCostl()
{
	return m_ItemInfo.m_iCost;
}

bool CSword::Equip()
{
	return false;
}

E_Item CSword::GetCategory()
{
	return m_ItemInfo.category;
}
