#pragma once
class CBmp
{
public:
	CBmp();
	~CBmp();
public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	HDC Get_MemDC() { return m_hMemDC; };
	void Load_Bmp(const TCHAR* pFilePath);

private:
	HDC m_hMemDC;
	HBITMAP m_hBitMap;
	HBITMAP m_hOldMap;
};

