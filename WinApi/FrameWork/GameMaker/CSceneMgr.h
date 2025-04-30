#pragma once
class CPlayer;
class CScene;

class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();
	CSceneMgr(CSceneMgr& rhs) = delete;
	CSceneMgr operator=(CSceneMgr& rhs) = delete;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	static CSceneMgr* Get_Instance() {
		if (!m_pInstance) {
			m_pInstance = new CSceneMgr;
		}
		return m_pInstance;
	}
	static void Destroy_Instance() {
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CSceneMgr* m_pInstance;
	CScene* m_NowScene;
};


