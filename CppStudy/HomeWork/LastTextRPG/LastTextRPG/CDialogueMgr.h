#pragma once
class CDialogueMgr
{
private: 
	CDialogueMgr();
	~CDialogueMgr();
public:
	CDialogueMgr(const CDialogueMgr& rhs) = delete;
	CDialogueMgr& operator=(const CDialogueMgr& rhs) = delete;

public :
	static CDialogueMgr& GetInstance() {
		static CDialogueMgr instance;
		return instance;
	}

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
	void SetMssg(const wstring& _msg);
private:
 wstring m_Message;
};

