#pragma once
class CkeyMgr
{

private:
	CkeyMgr();
	~CkeyMgr();
	CkeyMgr(CkeyMgr& rhs) = delete;
	CkeyMgr& operator=(CkeyMgr& rhs) = delete;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	bool isKeyDown(int _vkKey);
	bool isKeyTap(int _vkKey);
	bool isKeyHold(int _vkKey);
	bool isKeyAway(int _vkKey);

public:
	static CkeyMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CkeyMgr;
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CkeyMgr* m_pInstance;
	enum KEY_STATE {NONE,TAP,HOLD,AWAY};

	KEY_STATE m_keyArr[256];
};

