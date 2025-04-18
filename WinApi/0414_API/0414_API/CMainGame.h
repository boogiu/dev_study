#pragma once
class CObject;

class CMainGame
{
public: 
	CMainGame();
	~CMainGame();

public:
	void Intialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

private:
	HDC m_hDC;
	CObject* m_pPlayer;
};

