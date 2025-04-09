#pragma once

class CObj
{
public : 
	CObj();
	virtual ~CObj();

public :
	virtual void Release()						PURE;
	virtual void Initialize()						PURE;
	virtual void Update()						PURE;
	virtual void Render(HDC hDC)		PURE;

public:
	bool GetBorderOut();
	INFO GetPos();
	void SetPos(float _fX, float _fY);
	const RECT& GetRECT() const;
protected:
	void Update_Rect();
	void OutOfBorder();

protected:
	INFO m_tInfo;		//크기
	RECT m_tRect; //좌표
	float		m_fSpeed;// 이동 속도
	bool m_bOut;
};

