#pragma once
class CObject
{
public:
	CObject();
	virtual ~CObject();

public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC _hDC) PURE;
	virtual void Release() PURE;

public:
	void Set_Pos(float _x, float _y) { m_tInfo.fX = _x; m_tInfo.fY=_y; }
	void Set_Scale(float _x, float _y) { m_tInfo.fCX = _x; m_tInfo.fCY = _y; }
	INFO& Get_Pos() { return m_tInfo; }
	RECT& Get_Rect() { return m_tRendRC; }
	bool Is_Activate() { return m_bActive; }
	void Set_Active(bool _active) { m_bActive = _active; }

protected:
	void Update_RECT();
	//void Render_BMP(const TCHAR* pImageKey, HDC _hDC);
	void Render_BMP(const TCHAR* pImageKey, HDC _hDC, POINT index = { 0,0 });

protected:
	bool m_bActive;
	HDC memDC;
	RECT m_tRendRC;
	RECT m_tCollRC;
	VECTOR2 m_tVector;
	INFO m_tInfo;
};

