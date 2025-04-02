#include "pch.h"
#include "CThrowVenom.h"
#include "CGameMgr.h"
#include "CObject.h"
#include "CPlayer.h"

CThrowVenom::CThrowVenom(tagItem item)
{
	m_item = item;
	type = ITEM_TYPE::DISPOSABLE;
}

CThrowVenom::~CThrowVenom()
{
}

void CThrowVenom::Initialize()
{
}

void CThrowVenom::Release()
{
}

void CThrowVenom::Active()
{
	CObject* target=	CGameMgr::GetInstance().GetPlayer()->GetCompete();
	if (target) {
		target->GetDebuff(PoisonAtk, m_item.m_subPoint, m_item.m_Point);
	}
	else {
		wcout << L"대상이 존재하지 않습니다" << endl;
	}
}

void CThrowVenom::DeActive()
{
}
