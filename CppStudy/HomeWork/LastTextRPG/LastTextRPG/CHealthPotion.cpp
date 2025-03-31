#include "pch.h"
#include "CHealthPotion.h"
#include "CObject.h"

CHealthPotion::CHealthPotion(tagItem item)
{
	m_item = item;
}

CHealthPotion::~CHealthPotion()
{
}

void CHealthPotion::Initialize()
{
}

void CHealthPotion::Release()
{
}

void CHealthPotion::Active(CObject* obj)
{
	obj->Heal(m_item.m_Point);
}

void CHealthPotion::DeActive(CObject* obj)
{
}
