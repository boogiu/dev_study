#include "pch.h"
#include "CSlot.h"
#include "Items.h"
#include "CObject.h"

CSlot::CSlot()
	: m_Empty(true), m_Count(0), m_Item(nullptr)
{
}

CSlot::~CSlot()
{
}

void CSlot::Initialize()
{
}

void CSlot::Render()
{
	if (m_Item) {
		m_Item->RenderSummary();
	}
	else {
		wcout << "비어있음" << endl;
	}
}

void CSlot::Release()
{	
}

void CSlot::ActiveItem()
{
	if (Equipable()) {
		static_cast<CEquipable*>(m_Item)->Equip();
	}
	else {
		m_Count -= 1;
		static_cast<CDispoable*>(m_Item)->Active();
		if (m_Count == 0) {
			PopItem();
		}
	}
}


void CSlot::DeActiveItem()
{
	if (Equipable()) {
		static_cast<CEquipable*>(m_Item)->DeEquip();
	}
	else {
		static_cast<CDispoable*>(m_Item)->DeActive();
	}
}

ITEM_TYPE CSlot::GetItemType()
{
	return m_Item->type;
}

bool CSlot::Equipable()
{
	switch (m_Item->type)
	{
	case ITEM_TYPE::RIGHT:
	case ITEM_TYPE::LEFT:
	case ITEM_TYPE::TWOHAND:
	case ITEM_TYPE::HELMET:
	case ITEM_TYPE::ARMOR:
		return true;

	case ITEM_TYPE::NONE:
	case ITEM_TYPE::DISPOSABLE:
		return false;

	default:		
		return false;
	}
}

void CSlot::SwapEquip(CSlot& slot)
{
	//매개인자는 장비창

	CSlot tmpSlot = slot;
	slot = *this;
	*this = tmpSlot;

	slot.ActiveItem();
	if (this->m_Item) {
		this->DeActiveItem();
	}
}

bool CSlot::PushItem(CItem* item)
{
	if (item == nullptr) return false;
	if (item == m_Item &&item->type == ITEM_TYPE::DISPOSABLE)
	{//아이템이 있고, 일회용이라면 누적
		m_Count += 1;
		return true;
	}
	else if (!m_Item) { //아이템이 없다면
		m_Item = item;
		m_Count += 1;
		m_Empty = false;
		return true;
	}

	//아이템이 있다면
	return false;
}

void CSlot::PopItem()
{
	if (Equipable()) {
		if (static_cast<CEquipable*>(m_Item)->GetEquiped()) {
			static_cast<CEquipable*>(m_Item)->DeEquip();
		}
	}
	m_Item = nullptr;
	m_Count =0;
	m_Empty = true;
}