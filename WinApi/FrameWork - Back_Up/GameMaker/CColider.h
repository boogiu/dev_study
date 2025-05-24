#pragma once
class CObject;
class CColider
{
public:
	CColider();
	CColider(INFO _info);
	CColider(COLLIDER_INFO _info) :m_tInfo(_info){};

	~CColider();

public:
	void Render(HDC _hDC);
	void Update();
	void Late_Update();

public:
	bool isActive() { return m_bActive; };
	void SetActive(bool active) { m_bActive = active; };
	RECT& Get_Rect() { return m_tRect; }
	COLLIDER_INFO& Get_Info() { return m_tInfo; }

	void Update_Center(float _x, float _y) {
		m_tInfo.fX = _x; m_tInfo.fY = _y;
	}
	void Set_OffsetX(float left, float right) {
		m_tInfo.offsetLeft = left; m_tInfo.offsetRight = right;
	};
	void Set_OffsetY(float top, float bottom) {
		m_tInfo.offsetTop = top; m_tInfo.offsetBottom = bottom;
	};

private: 
	void Update_Rect();

private:
	bool m_bActive;
	COLLIDER_INFO m_tInfo;
	RECT m_tRect;
};

