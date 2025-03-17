#include "CMonster.h"
#include "pch.h"

CMonster::CMonster()
	:m_cName(""), m_iHp(0), m_iAtk(0)
{
}

CMonster::~CMonster()
{
}

void CMonster::Initialize(int index)
{
	strcpy_s(m_cName, sizeof(m_cName),"몬스터(임시)");
	m_iHp = 100;
	m_iAtk = 10;
}

void CMonster::GetDamage(int _dmg)
{
	m_iHp -= _dmg;
}
