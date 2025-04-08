#include "pch.h"
#include "CThrowBurn.h"

#include "CGameMgr.h"
#include "CObject.h"
#include "CPlayer.h"

CThrowBurn::CThrowBurn(tagItem item)
{
	m_item = item;
	type = ITEM_TYPE::DISPOSABLE;
}

CThrowBurn::~CThrowBurn()
{
}

void CThrowBurn::Initialize()
{
}

void CThrowBurn::Release()
{
}

void CThrowBurn::Active()
{
	CObject* target = CGameMgr::GetInstance().GetPlayer()->GetCompete();
	if (target) {
		target->GetDebuff(BurnAtk, m_item.m_subPoint, m_item.m_Point);
	}
	else {
		wcout << L"대상이 존재하지 않습니다" << endl;
	}
}

void CThrowBurn::DeActive()
{
}
