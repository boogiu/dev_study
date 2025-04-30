#pragma once
class CKeyMgr
{
private:
	CKeyMgr();
	~CKeyMgr();
	CKeyMgr(CKeyMgr& rhs) = delete;
	CKeyMgr operator = (CKeyMgr& rhs)=delete;

public:
	static CKeyMgr* Get_Instance() {
		if (!m_pInstance) {
			m_pInstance = new CKeyMgr;
		}
		return m_pInstance;
	}
	static void Destroy_Instance() {
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	bool GetKeyDown(int vk_Key);
	bool GetKeyTap(int vk_Key);
	bool GetKeyHold(int vk_Key);
	bool GetKeyAway(int vk_Key);

private:
	static CKeyMgr* m_pInstance;
	KEY_STATE m_KeyArr[VK_MAX];

};

