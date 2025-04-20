#pragma once
//싱글톤 클래스
/*
*모든 키 인풋을 받아오고 건네주는 싱글톤 클래스
*winAPI에서 지원하는 모든 키를 매핑하고 있음.
*/
class CInputMgr
{
private:
	CInputMgr();
	~CInputMgr();
	CInputMgr(CInputMgr& rhs) = delete;
	CInputMgr operator =(CInputMgr& rhs) = delete;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	bool KeyDown(int _vKey);
	bool KeyTap(int _vKey);
	bool KeyHold(int _vKey);
	bool KeyAway(int _vKey);

public:
	static CInputMgr* Get_Instance() {
		if (!m_InputMgr) {
			m_InputMgr = new CInputMgr;
		}
		return m_InputMgr;
	}

	static void Destroy_Instance() {
		if (m_InputMgr) {
			delete m_InputMgr;
		}
		m_InputMgr = nullptr;
	}
private:
	static CInputMgr* m_InputMgr;
	vector<KEY_STATE> m_vecKey;
};

