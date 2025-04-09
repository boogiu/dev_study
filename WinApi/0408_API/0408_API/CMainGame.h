#pragma once
class CObj;
class CKeyMgr;

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

private:
	void RenderFPS();
	void RenderPos();

	void UpdateBulletList();
	void UpdateMonsterList();
	void UpdateCollision();

private:
	TCHAR		m_szFPS[32];
	TCHAR		m_szPos[32];

	DWORD		m_dwTime;
	int			m_iFPS;

private:
	HDC m_hDC;
	CKeyMgr* m_KeyMgr;
	CObj* m_Player;
	list<CObj*> m_BulletList;
	list<CObj*> m_MonsterList;
};

