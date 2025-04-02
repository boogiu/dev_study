#include "pch.h"
#include "CHealthPotion.h"
#include "CGameMgr.h"
#include "CPlayer.h"

CHealthPotion::CHealthPotion(tagItem item)
{
	m_item = item;
	type = ITEM_TYPE::DISPOSABLE;
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

void CHealthPotion::Active()
{
	CGameMgr::GetInstance().GetPlayer()->Heal(m_item.m_Point);
}

void CHealthPotion::DeActive()
{
}
