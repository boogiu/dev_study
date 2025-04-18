#pragma once
class CButton
{
public:
	CButton();
	~CButton();

public:
	void Initialize(const TCHAR* buffer);
	void Update();
	void Render(HDC _hDC);
	void Release();

public:
	void SetText(const TCHAR* buffer);
	RECT GetRect() { return m_tRect; }
private:
	void Update_Rect();
private:
	RECT m_tRect;
	INFO m_tInfo;
	TCHAR m_Buffer[64];
};

