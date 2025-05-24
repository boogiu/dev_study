#include "pch.h"
#include "CTimeManager.h"

CTimeManager::CTimeManager()
{
}

CTimeManager::~CTimeManager()
{
}

void CTimeManager::Initialize()
{
	QueryPerformanceFrequency(&m_nFrequency);
	QueryPerformanceCounter(&m_nPrevCnt);
}

void CTimeManager::Update()
{
	QueryPerformanceCounter(&m_nCurCnt);
	LONGLONG CounterDiff = (m_nCurCnt.QuadPart - m_nPrevCnt.QuadPart); // 이전 카운터와 현재 카운터의 차이

	m_fDeltaTime = static_cast<float>(CounterDiff) / static_cast<float>(m_nFrequency.QuadPart);

	// 안정성 보정: 델타가 너무 크거나 작을 경우 제한
	if (m_fDeltaTime < 0.f) m_fDeltaTime = 0.f;
	if (m_fDeltaTime > 0.1f) m_fDeltaTime = 0.1f;

	// FPS 측정 (1초마다)
	m_iFrameCount++;
	m_fFPSTimeAcc += m_fDeltaTime;
	if (m_fFPSTimeAcc >= 1.f)
	{
		m_iFPS = m_iFrameCount;
		m_iFrameCount = 0;
		m_fFPSTimeAcc = 0.f;
	}

	m_nPrevCnt = m_nCurCnt;
}

void CTimeManager::Late_Update()
{
}

void CTimeManager::Render()
{
}

void CTimeManager::Release()
{
}
