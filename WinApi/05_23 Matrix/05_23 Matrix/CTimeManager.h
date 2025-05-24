#pragma once
class CTimeManager
{
private:
	CTimeManager();
	~CTimeManager();

public:
	static CTimeManager& Get_Instance() {
		static CTimeManager instance;
		return instance;
	}

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

public:
	float Get_Delta() { return m_fDeltaTime*m_TimeScale; }

private:
	float m_TimeScale = 1.f;
	float m_fDeltaTime = 0.f;
	int m_iFPS = 0;
	int m_iFrameCount = 0;;
	float m_fFPSTimeAcc = 0.f;
	LARGE_INTEGER m_nCurCnt;
	LARGE_INTEGER m_nPrevCnt;
	LARGE_INTEGER m_nFrequency;

};

