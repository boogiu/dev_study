#include "pch.h"
#include "CEquip.h"
#include "CSlot.h"
#include "CItem.h"

CEquip::CEquip()
{
}

CEquip::~CEquip()
{
}

void CEquip::Initialize()
{
	m_mapSlot.insert({ ITEM_TYPE::HELMET, new CSlot(ITEM_TYPE::HELMET) });
	m_mapSlot.insert({ ITEM_TYPE::LEFT, new CSlot(ITEM_TYPE::LEFT) });
	m_mapSlot.insert({ ITEM_TYPE::RIGHT, new CSlot(ITEM_TYPE::RIGHT) });
	m_mapSlot.insert({ ITEM_TYPE::ARMOR, new CSlot(ITEM_TYPE::ARMOR) });
}

void CEquip::Release()
{
	unordered_map<ITEM_TYPE, CSlot*>::iterator iter = m_mapSlot.begin();
	for (;iter != m_mapSlot.end(); ++iter) {
		SAFE_DELETE(iter->second);
	}
}

void CEquip::ShowInven()
{
	wcout << L"  Çï¸ä" ;
	m_mapSlot[ITEM_TYPE::HELMET]->Render();

	wcout << L"  ¿Þ¼Õ";
	m_mapSlot[ITEM_TYPE::LEFT]->Render();

	wcout << L"  ¿À¸¥¼Õ";
	m_mapSlot[ITEM_TYPE::RIGHT]->Render();

	wcout << L"  °©¿Ê";
	m_mapSlot[ITEM_TYPE::ARMOR]->Render();
}

bool CEquip::PushItem(CItem* _item)
{
	if (m_mapSlot.find(_item->type) == m_mapSlot.end()) {
		return false;
	}
	else if (m_mapSlot[_item->type] != nullptr) {
		return false;
	}
	else {
		m_mapSlot[_item->type]->PushItem(_item);
	}
	return true;
}

void CEquip::PopItem(ITEM_TYPE _type)
{
	m_mapSlot[_type]->PopItem();
}