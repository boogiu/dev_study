#include "pch.h"
#include "CStaff.h"

#include "CGameMgr.h"
#include "CObject.h"
#include "CPlayer.h"

CStaff::CStaff(tagItem item)
{
	m_item = item;
	type = ITEM_TYPE::RIGHT;
}

CStaff::~CStaff()
{
}

void CStaff::Initialize()
{
}

void CStaff::Release()
{
}

void CStaff::Equip()
{
	CPlayer* target = CGameMgr::GetInstance().GetPlayer();
	if (target) {
		target->GetInfo().iMgAtk += m_item.m_Point;
		SetEquiped(true);
	}
}

void CStaff::DeEquip()
{
	CPlayer* target = CGameMgr::GetInstance().GetPlayer();
	if (target) {
		target->GetInfo().iMgAtk -= m_item.m_Point;
		SetEquiped(false);
	}
}
