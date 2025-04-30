#include "pch.h"
#include "CTimeMgr.h"

CTimeMgr* CTimeMgr::m_pInstance = nullptr;

CTimeMgr::CTimeMgr()
	:m_iFPS(0),m_fDeltaTime(0.f)
{
	ZeroMemory(m_szBuffer, sizeof(m_szBuffer));
}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::Initialize()
{
	QueryPerformanceFrequency(&m_nFrequency);
	QueryPerformanceCounter(&m_nPrevCnt);
}

void CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_nCurCnt);
	LONGLONG CounterDiff = (m_nCurCnt.QuadPart - m_nPrevCnt.QuadPart); // 이전 카운터와 현재 카운터의 차이
	m_fDeltaTime = static_cast<float>(CounterDiff) / static_cast<float>(m_nFrequency.QuadPart);
	m_iFPS = static_cast<int>(1.f / m_fDeltaTime);	 //프리퀀시가 초당 n개 / 지금 1개 당 초 
	m_nPrevCnt = m_nCurCnt;
}

void CTimeMgr::Late_Update()
{
}

void CTimeMgr::Render(HDC _hDC)
{
	swprintf_s(m_szBuffer, L"FPS : %d", m_iFPS);
	SetWindowText(g_hWnd, m_szBuffer);
}

void CTimeMgr::Release()
{
}
