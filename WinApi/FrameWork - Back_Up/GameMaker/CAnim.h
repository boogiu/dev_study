#pragma once
class CObject;

class CAnim
{
public:
	CAnim(CObject* _Owner) :ElapsedTime(0.f),m_bAnimEnd(false)
	{ m_pOwner = _Owner; };
	~CAnim();

public:
	void Set_Anim(const TCHAR * pFilePath, ANI_FRAME anim);
	void Update( );
	void Late_Update();
	void Render(HDC _hDC);
	void Release();
	
public :
	bool isAnimEnd() { return m_bAnimEnd; }
	bool isAnimStart() { return m_nowPoint.x == m_aniInfo.startPoint.x; }

	void ChangeOnlyPath(const TCHAR* pFilePath);
	void ChangeXFrame(int _x) { m_nowPoint.x = _x; }
	void ChangeYFrame(int _y) { m_nowPoint.y = _y; }

private:
	bool m_bAnimEnd;
	float ElapsedTime;


	POINT m_nowPoint;
	ANI_FRAME m_aniInfo;

	CObject* m_pOwner;
	HDC memDC;
};

