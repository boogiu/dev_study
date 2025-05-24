#pragma once
class CObject;
class CAnim
{
public:
	CAnim(CObject* _Owner) :ElapsedTime(0.f), m_bAnimEnd(false), m_aniInfo{},m_bStop(false),m_nowPoint{},memDC(nullptr),m_BreakPoint{},m_bhaveBreak(false)
	{ m_pOwnObj = _Owner; };

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
	POINT Get_NowFrame() { return m_nowPoint; }
	void ChangeOnlyPath(const TCHAR* pFilePath);
	void ChangeXFrame(int _x) { m_nowPoint.x = _x; }
	void ChangeYFrame(int _y) { m_nowPoint.y = _y; }
	void Set_Stop(bool _stop);
	bool Calc_NowFrame(POINT X);
	void Set_BreakPoint(POINT X);
	bool Less_thanFrame(POINT X);
private:
	bool m_bAnimEnd;
	float ElapsedTime;
	bool m_bStop;

	POINT m_nowPoint;
	POINT m_BreakPoint;
	bool m_bhaveBreak;
	ANI_FRAME m_aniInfo;
	SIZE m_fullTextureSize;
	CObject* m_pOwnObj;
	HDC memDC;
};


// 헤더에 선언
namespace AnimUtils {
	POINT IndexToFrame(int index, int framePerRow);
	int FrameToIndex(POINT frame, int framePerRow);
	int FrameRangeToCount(POINT start, POINT end, int framePerRow);
	SIZE GetBitmapSizeFromDC(HDC hMemDC);
}
