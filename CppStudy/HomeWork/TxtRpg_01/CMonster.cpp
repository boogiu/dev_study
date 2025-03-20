#include "CMonster.h"
#include "pch.h"

CMonster::CMonster()
	:m_cName{}
{
	memset(&m_mInfo, 0, sizeof(m_mInfo));
}

CMonster::~CMonster()
{
}

void CMonster::Init(int index)
{
	strcpy_s(m_cName, sizeof(m_cName), "몬스터");
	m_mInfo.m_iAtk = (index + 1) * 5;
	m_mInfo.m_iMaxHp = (index + 1) * 5;
	m_mInfo.m_iNowHp =m_mInfo.m_iMaxHp;
}

void CMonster::Render()
{
}

void CMonster::Release()
{
}


void CMonster::ShowStat()
{
	cout <<m_cName << endl;
	cout << "체력 : " << m_mInfo.m_iNowHp << "/" << m_mInfo.m_iMaxHp << endl;
	cout << "공격력 : " << m_mInfo.m_iAtk << endl;
}

void CMonster::GetDamage(int dmg)
{
	cout << m_cName << "은 " << dmg << "만큼의 피해를 입었다!" << endl;
	if (m_mInfo.m_iNowHp - dmg <= 0) {
		m_mInfo.m_iNowHp = 0;
	}
	else {
		m_mInfo.m_iNowHp -= dmg;
	}
}

void CMonster::Restore()
{
	m_mInfo.m_iNowHp = m_mInfo.m_iMaxHp;
}
