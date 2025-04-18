#pragma once
class CScene;

class CSceneMgr : public Singleton<CSceneMgr>
{
	friend class Singleton;
private:
	CSceneMgr();
	~CSceneMgr();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	void ChangeScene(SCENE_ID _to, SCENE_ID _from);
private:
	
	SCENE_ID m_eNowScene;
	vector<CScene*> m_vecScenes;
};

