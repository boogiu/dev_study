#pragma once
class CInputMgr : public Singleton<CInputMgr>
{
	friend class Singleton;
private:
	CInputMgr();
	~CInputMgr();
	CInputMgr(CInputMgr& rhs) = delete;
	CInputMgr& operator=(CInputMgr& rhs) = delete;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

public:
	bool GetKeyDown(int _vk_key);
	bool GetKeyTap(int _vk_key);
	bool GetKeyHold(int _vk_key);
	bool GetKeyAway(int _vk_key);


private:
	array<KEY_STATE,256> m_KeyArray;
};

