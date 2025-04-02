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
	nowHp = m_Stat.iMaxHp;
	StateRestore();
	m_AtkType |= NormalAtk;
	m_State = 0;
}

void CMonster::Render()
{
	wcout << endl;
	wcout << m_Sname << RenderState()<<endl;
	wcout << L"-----------" << endl;
	wcout << L"체력 : "<<nowHp<<L" / " << m_Stat.iMaxHp << endl;
	wcout << L"공격력 : " << m_Stat.iNormAtk<<endl;
	wcout << L"===================================" << endl;

}

void CMonster::Release()
{
}
