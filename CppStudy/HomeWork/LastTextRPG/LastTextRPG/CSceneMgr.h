#pragma once
class CScene;

class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();
public:
	CSceneMgr(const CSceneMgr& rhs) = delete;
	CSceneMgr& operator = (const CSceneMgr& rhs) = delete;

public:
	//SINGLETON
	static CSceneMgr& GetInstance() {
		static CSceneMgr instance;
		return instance;
	}
public:
	void Initialize();
	void Release();
	void Update();
	void Render();
public:
	void ChangeScene(SCENE _scene);
	SCENE GetNowScene() { return nowScene; }
private:
	vector<CScene*> m_SceneArr;
	SCENE nowScene;
};

