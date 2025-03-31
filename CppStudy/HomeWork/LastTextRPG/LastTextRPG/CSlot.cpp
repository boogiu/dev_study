#include "pch.h"
#include "CSlot.h"
#include "CItem.h"
#include "CObject.h"

CSlot::CSlot()
	:m_Empty(true), m_Count(0), m_Item(nullptr)
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
		wcout <<m_Count<< L"개" << endl;
	}
	else {
		wcout << "비어있음" << endl;
	}
}

void CSlot::Release()
{	
}

void CSlot::ActiveItem(CObject* obj)
{
	if (!m_Item) { return; }

	m_Item->Active(obj);
	--m_Count;

	if (m_Count == 0) {
		m_Item = nullptr;
		m_Empty = true;
	}
}

void CSlot::PushItem(CItem* item)
{
	if (!m_Item) {
		m_Item = item;
	}

	if (m_Item != item) {
		return;
	}
	
	m_Empty = false;
	m_Count += 1;
}

void CSlot::PopItem()
{
	m_Count = 0;
	m_Item = nullptr;
	m_Empty = true;
}

