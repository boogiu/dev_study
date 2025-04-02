#include "pch.h"
#include "CSword.h"
#include "CGameMgr.h"
#include "CObject.h"
#include "CPlayer.h"

CSword::CSword(tagItem item)
{
	m_item = item;
	type = ITEM_TYPE::RIGHT;
}

CSword::~CSword()
{
}

void CSword::Initialize()
{
}

void CSword::Release()
{
}

void CSword::Equip()
{
	CPlayer* target = CGameMgr::GetInstance().GetPlayer();
	if (target) {
			target->GetInfo().iNormAtk += m_item.m_Point;
			SetEquiped(true);
	}
}

void CSword::DeEquip()
{
	CPlayer* target = CGameMgr::GetInstance().GetPlayer();
	if (target) {
		target->GetInfo().iNormAtk -= m_item.m_Point;
		SetEquiped(false);
	}
	
}
