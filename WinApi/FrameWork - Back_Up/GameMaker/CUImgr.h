#pragma once
class CBase_UI;
class CCamera;
class CObject;

class CUIMgr
{
private:
	CUIMgr();
	~CUIMgr();
	CUIMgr(CUIMgr& rhs) = delete;
	CUIMgr operator=(CUIMgr& rhs) = delete;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	static CUIMgr* Get_Instance() {
		if (!m_pInstance) {
			m_pInstance = new CUIMgr;
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
	static CUIMgr* m_pInstance;
	list <CBase_UI*> m_UI_List;
	CObject* m_pTarget;
};

