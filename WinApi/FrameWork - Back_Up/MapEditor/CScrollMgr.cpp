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
	/*if (2.f < m_fScrollX)
		m_fScrollX = 0.f;

	if (2.f < m_fScrollY)
		m_fScrollY = 0.f;

	if (WINCX - 1918 > m_fScrollX)
		m_fScrollX = WINCX - 1920;

	if (WINCY - 1280 > m_fScrollY)
		m_fScrollY = WINCY - 1280;*/

}