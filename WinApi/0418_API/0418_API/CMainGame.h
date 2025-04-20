#pragma once

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

public:
	inline bool GetGameEnd() { return m_bGameEnd; };
	inline void SetGameEnd(bool _GameEnd) { m_bGameEnd = _GameEnd; };

private:
	//게임 종료 체크
	bool m_bGameEnd;
	HDC m_hDC;

};
