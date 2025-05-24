#pragma once
class CBase_UI
{
public:
	CBase_UI();
	virtual ~CBase_UI();
public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC _hDC)PURE;
	virtual void Release()PURE;
public:
	void Set_Info(UI_INFO _info) { m_tInfo = _info; };

protected:
	HDC memDC;
	RECT m_tRect;
	UI_INFO m_tInfo;
};

