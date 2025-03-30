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
}

void CMonster::Update()
{
}

void CMonster::Render()
{
}

void CMonster::Release()
{
}
