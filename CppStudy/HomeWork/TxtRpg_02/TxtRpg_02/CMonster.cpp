#include "pch.h"
#include "CMonster.h"

CMonster::CMonster(int _level)
	:m_cName(nullptr), m_iLevel(_level)
{
}

CMonster::CMonster(int _level, char* Name, tagStat _Stat)
	:m_cName(nullptr), m_iLevel(_level)
{
	m_cName = Name;
	m_iLevel = _level;
	m_Stat = _Stat;
}

CMonster::~CMonster()
{
}

void CMonster::Init()
{

}

void CMonster::Release()
{
	SAFE_DELETE(m_cName);
}
