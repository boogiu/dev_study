#include "pch.h"
#include "pch.h"
#include "CItem.h"

CItem::CItem()
{
}

CItem::~CItem()
{
}

void CItem::Initialize()
{
}

void CItem::Release()
{
}

void CItem::Render()
{
	wcout << m_item.m_Name<<L" (" <<m_item.m_Level<<L")" << endl;
	wcout << m_item.m_Desc << endl;
	wcout << m_item.m_Cost <<"GOLD" << endl;
}

void CItem::RenderSummary()
{
	wcout << m_item.m_Name << L" (" << m_item.m_Level << L")";
}
