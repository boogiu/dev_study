#include "CMonster.h"

CMonster::CMonster(int _level)
	:m_cName(" ¸ó½ºÅÍ"), m_iLevel(_level)
{
}

CMonster::~CMonster()
{
}

void CMonster::Init()
{
	m_Stat.m_iMaxHp = m_iLevel * 10;
	m_Stat.m_iNowHp = m_Stat.m_iMaxHp;

	m_Stat.m_iAtk = 2*m_iLevel;
	m_Stat.m_iDef = 2 * m_iLevel;

	m_Stat.m_iCrit = 5 * m_iLevel;
	if (m_Stat.m_iCrit > 100) { m_Stat.m_iCrit = 100; }
	m_Stat.m_fCritDgm = 10*m_iLevel;

	m_Stat.m_iAvd = 5*m_iLevel;

	m_Stat.m_iExp = 3*m_iLevel;
	m_Stat.m_iGold = 10*m_iLevel;
}

void CMonster::Release()
{
}
