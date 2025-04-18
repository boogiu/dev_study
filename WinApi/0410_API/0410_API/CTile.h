#pragma once
class CTile
{
public:
	CTile();
	virtual ~CTile();

public:
	virtual void Initialize()		PURE;
	virtual void Update()		PURE;
	virtual void Render(HDC _hDC)		PURE;
	virtual void Release()		PURE;

protected:
	void Update_Rect();

public: 
	RECT& GetRect() { return m_tRect; }
	INFO& GetPos() { return m_tInfo; }

protected:
	INFO m_tInfo;
	RECT m_tRect;
};

