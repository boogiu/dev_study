#pragma once
#include "CCamera.h"

class CColider;
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
	virtual void OnCollisionEnter(CObject* object, RECT collRECT);
	CColider* Get_Collider() { return m_Colider; }

public:
	void Set_Pos(float _x, float _y) { m_tInfo.fX = _x; m_tInfo.fY=_y; }
	INFO_EX& Get_Info() { return m_tInfo; }
	RECT& Get_Rect() { return m_tRendRC; }

	void Add_Velocity(float _x, float _y){m_tVector.fX += _x;m_tVector.fY += _y;}
	void Set_Velocity(float _x, float _y) { m_tVector.fX = _x;m_tVector.fY = _y; }
	VECTOR2 Get_Velocity() { return m_tVector; }

	bool Is_Activate() { return m_bActive; }
	void Set_Active(bool _active) { m_bActive = _active; }

	int Get_Dir() { return m_Dir; }
	void Set_Dir(int i) { m_Dir = i; }
	void Set_Grounded(bool ground) { m_bGround = ground; }

protected:
	void Update_RECT();
	void Render_BMP(const TCHAR* pImageKey, HDC _hDC, POINT index = { 0,0 });

protected:
	bool m_bActive;
	bool m_bGround;

	int m_Dir; // 1¿À¸¥ÂÊ -1¿ÞÂÊ

	HDC memDC;
	RECT m_tRendRC;
	VECTOR2 m_tVector;
	INFO_EX m_tInfo;

	CColider* m_Colider;
};

