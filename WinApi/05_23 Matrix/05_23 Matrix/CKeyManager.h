#pragma once
class CKeyManager
{
private:
	CKeyManager();
	~CKeyManager();

public:
	static CKeyManager& Get_Instance() {
		static CKeyManager instance;
		return instance;
	}

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Release();

public:
	bool Key_Down(int _vk);
	bool Key_Tap(int _vk);
	bool Key_Hold(int _vk);
	bool Key_Away(int _vk);
	POINT Get_Cursor();
private:
	std::vector<KeyState> m_KeyContainer;
	POINT m_ptMouse;
};

