#include "CTimer.h"

CTimer::CTimer() : m_fTimeDelta(0.f)
{

	ZeroMemory(&m_FixTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_FrameTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}

CTimer::~CTimer()
{
}

HRESULT CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceCounter(&m_FrameTime);

	QueryPerformanceFrequency(&m_CpuTick);
	return S_OK;
}

void CTimer::Update_Timer()
{
	QueryPerformanceCounter(&m_FrameTime);

	if (m_FrameTime.QuadPart - m_FixTime.QuadPart > m_CpuTick.QuadPart)
	{ //이번 프레임 - 픽스 타임이 1초 기준을 넘었다면?
		//QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;//다시 픽스 타임을 이번 프레임 타임으로
	}

	m_fTimeDelta = (m_FrameTime.QuadPart - m_LastTime.QuadPart);
	m_LastTime = m_FrameTime;
}

CTimer* CTimer::Create()
{
	CTimer* instance = new CTimer;
	if (FAILED(instance->Ready_Timer())) {
		Engine::Safe_Release(instance);
		//초기화 실패 시 객체 삭제
	}
	return instance;
}

void CTimer::Free()
{
}
