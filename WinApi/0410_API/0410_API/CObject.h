#pragma once
class   CTile;

class CObject
{
public:
	CObject();
	virtual ~CObject() PURE;

public:
	virtual void Initialize()							PURE;
	virtual void Update()							PURE	;
	virtual void Render(HDC _hDC)		PURE;
	virtual void Release()							PURE;

public:
	virtual void Activate() PURE;
	virtual void DeActivte() PURE;

	virtual void OnCollisionEnter(CObject* _object);
	virtual void OnCollisionEnter(CTile* _tile);
	
	virtual void OnCollisionOut(CObject* _object);
	virtual void OnCollisionOut(CTile* _tile);

public:
	RECT& GetRect() { return m_tRect; }
	INFO& GetPos() { return m_tInfo; }
	VELOCITY& GetVelocity(){ return m_tVelocity; }
	
	void SetPos(float _x, float _y) { m_tInfo.fX = _x; m_tInfo.fY = _y; }

	bool GetActivate() { return m_bActive; }
	short GetState() { return m_sState; }
	
	bool	IsOut() { return m_bOut; }

protected:
	void Update_Rect();
	void BorderOutCheck();
	virtual void OnHit();

protected:
	bool m_bOut;
	bool m_bActive;
	float m_fSpeed;

	VELOCITY m_tVelocity;
	INFO m_tInfo;
	RECT m_tRect;

	short m_sState;
};

