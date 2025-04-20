#pragma once
//싱글톤 클래스

/*전방선언*/
class CScene;

/*
* 씬들의 전환과 관계를 관리하는 중재자 클래스
*/
class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();
	CSceneMgr(CSceneMgr& rhs) = delete;
	CSceneMgr operator=(CSceneMgr& rhs) = delete;

public:
	void Intialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();
public:
	void ChangeScene(wstring _sceneID);
public: 
	//싱글톤 인스턴스 호출
	static CSceneMgr* Get_Instance() {
		if (!m_SceneMgr) 		{
			m_SceneMgr = new CSceneMgr;
		}
		return m_SceneMgr;
	}

	static void Destroy_Instance() {
		if (m_SceneMgr) {
			delete m_SceneMgr;
		}
		m_SceneMgr = nullptr;
	}

private:
	bool IsSceneIDValid(wstring _sceneID);

	/* 씬의 아이디와 객체를 받아와서 생성을 하게 되는 클래스 템플릿*/
	template<typename T>
	void RegisterScene(wstring _sceneID) {
		if (!IsSceneIDValid(_sceneID)) return;
		CScene* _scene = new T;
		_scene->Initialize();
		m_umapScene.insert({ _sceneID, _scene });
	}

private:
	//인스턴스를 가리키는 포인터
	static CSceneMgr* m_SceneMgr;

	//현재 업데이트 중인 씬 
	wstring m_SceneID;

	//씬 배열의 관리 맵
	unordered_map <wstring, CScene*> m_umapScene;

};
