#include "pch.h"
#include "CObject.h"

CObject::CObject()
	:m_Stat{0}
{

}

CObject::~CObject()
{
}

void CObject::Init()
{
}

void CObject::Render()
{
	std::cout <<"-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_" << std::endl;
	std::cout << GetName()<< std::endl;
	std::cout << "------------------" << std::endl;
	std::cout << "공격력 : "<<m_Stat.m_iAtk<<"\t방어력 : "<<m_Stat.m_iDef << std::endl;
	std::cout << "체력 : " << m_Stat.m_iNowHp << "/ " << m_Stat.m_iMaxHp << std::endl;
	std::cout << "크리티컬 확률: " << m_Stat.m_iCrit << "%\t회피율 : " << m_Stat.m_iDef <<"%" << std::endl;
	std::cout << "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_" << std::endl;
}

void CObject::Release()
{
}

char* CObject::GetName()
{
	return nullptr;
}

int CObject::GetAtk()
{

	SetRandom;
	int tmp =Random(101);

	if (m_Stat.m_iCrit >= tmp) {
		std::cout << GetName() << "의 치명타 적중!!" << std::endl;
		return (int)m_Stat.m_iAtk * (1.f + (float)m_Stat.m_iCrit / 100);
	}
	return m_Stat.m_iAtk;
}

void CObject::GetDamage(int _dmg)
{
	SetRandom;
	int tmp = Random(101);

	if (m_Stat.m_iAvd >= tmp) {
		std::cout << GetName() << "의 완벽한 회피!!" << std::endl;
		return;
	}

	int iDmg = (_dmg - m_Stat.m_iDef);
	if (iDmg <= 0) {
		std::cout << GetName() << "은 전혀 피해를 입지 않았다!" << std::endl;
		return;
	}

	m_Stat.m_iNowHp -= iDmg;
	std::cout << GetName() << "은 "<< iDmg<<"만큼의 피해를 입었다!" << std::endl;
}

void CObject::Heal(int _heal)
{
	m_Stat.m_iNowHp += _heal;
	if (m_Stat.m_iNowHp > m_Stat.m_iMaxHp) {
		m_Stat.m_iNowHp = m_Stat.m_iMaxHp;
	}
}

tagStat CObject::GetStatus()
{
	return m_Stat;
}
