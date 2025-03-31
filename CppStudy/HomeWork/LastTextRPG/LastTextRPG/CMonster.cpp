#include "pch.h"
#include "CMonster.h"

CMonster::CMonster(const tagStatus& stat, const std::wstring& name)
{
	m_Sname = name;
	m_Stat = stat;
}

CMonster::~CMonster()
{
}

void CMonster::Initialize()
{
	m_AtkType |= NormalAtk;
	nowHp = m_Stat.iMaxHp;
}

void CMonster::Render()
{
	wcout << m_Sname << RenderState()<<endl;
	wcout << L"-----------" << endl;
	wcout << L"체력 : "<<nowHp<<L" / " << m_Stat.iMaxHp << endl;
	wcout << L"공격력 : " << m_Stat.iNormAtk<<endl;
	wcout << L"===================================" << endl;

}

void CMonster::Release()
{
}
