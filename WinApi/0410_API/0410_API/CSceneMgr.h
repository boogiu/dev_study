#pragma once

class CScene;
class CObject;
class CBorder;
class CSceneMgr : public Singleton<CSceneMgr>
{
	friend class Singleton;
private:
	CSceneMgr();
	~CSceneMgr();

public:
	void Initialize();
	void Update();
	void Render(HDC _hDC);
	void Release();

public : 
	void ChangeScene(SCENE _nextScene);
	SCENE GetNowScene() { return m_eNowScene; }

private:
	vector<CScene*> m_vecScene;
	SCENE m_eNowScene;
	CBorder* m_Border;
};

