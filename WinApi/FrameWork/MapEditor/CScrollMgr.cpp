#include "pch.h"
#include "CScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr()
	:m_fScrollX(0.f), m_fScrollY(0.f)
{
}

CScrollMgr::~CScrollMgr()
{
	Release();
}

void CScrollMgr::Initialize()
{
}

void CScrollMgr::Update()
{
}

void CScrollMgr::Late_Update()
{
}

void CScrollMgr::Render(HDC _hDC)
{
}

void CScrollMgr::Release()
{
}

void CScrollMgr::Scroll_Lock()
{
	if (m_fScrollX > 100)
	{
		m_fScrollX = 100;
	}
	if (m_fScrollX < -(MAP_CX + 100))
	{
		m_fScrollX = -(MAP_CX + 100);
	}
	if (m_fScrollY > 0)
	{
		m_fScrollY = 0;
	}
	if (m_fScrollY < -MAP_CY)
	{
		m_fScrollY =-MAP_CY;
	}
}