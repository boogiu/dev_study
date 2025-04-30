#pragma once
class CBmp;

class CBmpMgr
{
	friend class CResourceMgr;
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
private:
	unordered_map<const TCHAR*, CBmp*> m_mapBit;

};

