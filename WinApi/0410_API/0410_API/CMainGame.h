#pragma once
class CObject;
class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public: 
	void Initialize();
	void Update();
	void Render();
	void Release();

public: 
	bool GetEnd() { return m_bGameEnd; }

private:
	void RenderFrame();
	void CheckEnd();
private: 
	//UI
	TCHAR szFrame[32];
	ULONGLONG m_tickCount;
	int m_iFrame;

	 //Data
	HDC m_hDC;
	bool m_bGameEnd;
};

