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
}

void CObject::Release()
{
}

int CObject::GetAtk()
{
	return 0;
}

void CObject::GetDamage(int _dmg)
{
}

void CObject::Heal(int _heal)
{
}

tagStat CObject::GetStatus()
{
	return tagStat();
}
