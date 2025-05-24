#pragma once
#include "CCamera.h"
#include "CCollider.h"

class CCollider;
class CAttackBox;
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
	virtual void OnCollisionEnter(CObject* object, RECT collRECT,HIT_TYPE _hitType);
	virtual void OnCollisionWall(int dir);
	CCollider* Get_Collider() { return m_pCollider; }

public:
	void Set_Pos(float _x, float _y) { m_tInfo.fX = _x; m_tInfo.fY=_y; }
	INFO_EX& Get_Info() { return m_tInfo; }
	RECT& Get_Rect() { return m_tRect; }

	void Add_Velocity(float _x, float _y){m_tVector.fX += _x;m_tVector.fY += _y;}
	void Set_Velocity(float _x, float _y) { m_tVector.fX = _x;m_tVector.fY = _y; }
	void Set_VelocityY( float _y) { m_tVector.fY = _y; }
	void Set_VelocityX( float _x) { m_tVector.fX = _x; }

	VECTOR2 Get_Velocity() { return m_tVector; }

	bool Is_Activate() { return m_bActive; }
	virtual void Set_Active(bool _active) { m_bActive = _active; }

	int Get_Dir() { return m_Dir; }
	void Set_Dir(int i) { m_Dir = i; }
	void Set_Grounded(bool ground) { m_bGround = ground; }

	void Set_MoveSpeed(float _speed) { m_fMoveSpeed = _speed; };
	float Get_MoveSpeed() { return m_fMoveSpeed; }
	bool isOnGround();
	void Set_MoveSCLimit(bool limit) { m_bMoveLimit = limit; }
	bool Get_Coll_Active() { return m_bCollider; }
	void Set_Coll_Active(bool active) { m_bCollider = active; }

protected:
	void Update_RECT();
	void Render_BMP(const TCHAR* pImageKey, HDC _hDC, POINT index = { 0,0 });

protected:
	bool m_bCollider;
	bool m_bActive;
	bool m_bGround;
	bool m_bMoveLimit;

	int m_Dir; // 1¿À¸¥ÂÊ -1¿ÞÂÊ
	float m_fMoveSpeed;

	HDC memDC;
	RECT m_tRect;
	VECTOR2 m_tVector;
	INFO_EX m_tInfo;
	CCollider* m_pCollider;
	CAttackBox* m_pAtkBox;
};

