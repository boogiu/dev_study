#pragma once
class CBmp;

class CBmpMgr
{
private:
	CBmpMgr();
	~CBmpMgr();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	HDC Find_Image(const TCHAR* pImageKey);
	void Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImageKey);
public:
	static CBmpMgr* Get_Instance() {
		if (!m_pInstance) {
			m_pInstance = new CBmpMgr;
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
	static CBmpMgr* m_pInstance;
	unordered_map<const TCHAR*, CBmp*> m_mapBit;

};

