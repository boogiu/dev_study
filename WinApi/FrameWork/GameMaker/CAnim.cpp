#include "pch.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CResourceMgr.h"
#include "CObject.h"
#include "CUImgr.h"
#include"CCamera.h"
#include"CBaseEffect.h"

CAnim::~CAnim()
{
}

void CAnim::Set_Anim(const TCHAR* pFilePath, ANI_FRAME anim)
{
	memDC = CResourceMgr::Get_Instance()->Find_Image(pFilePath);
	m_aniInfo = anim;
	m_nowPoint = m_aniInfo.startPoint;
	m_bAnimEnd = false;
	m_bStop = false;
	ElapsedTime = 0;
	m_BreakPoint = { 0,0 };
	m_bhaveBreak = false;
	m_fullTextureSize = AnimUtils::GetBitmapSizeFromDC(memDC);
}

//void CAnim::Update()
//{
//	if (m_bStop) return;
//
//	ElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
//
//	if (ElapsedTime > m_aniInfo.frameSpeed)
//	{
//		m_nowPoint.x += 1;
//		ElapsedTime = 0.f;
//
//		// x 초과 시
//		if (m_nowPoint.x > m_aniInfo.endPoint.x)
//		{
//			m_nowPoint.x = 0;
//			m_nowPoint.y += 1;
//
//			// y도 초과 시
//			if (m_nowPoint.y > m_aniInfo.endPoint.y)
//			{
//				if (m_aniInfo.isLoop)
//					m_nowPoint = m_aniInfo.LoopPoint;
//				else
//					m_nowPoint = m_aniInfo.endPoint;
//			}
//		}
//	}
//}

void CAnim::Update()
{
	if (m_bStop) return;

	if (m_bhaveBreak && Calc_NowFrame(m_BreakPoint)) return;

	m_fullTextureSize = AnimUtils::GetBitmapSizeFromDC(memDC);

	ElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();

	if (ElapsedTime > m_aniInfo.frameSpeed)
	{

		int framePerRow = m_fullTextureSize.cx / m_aniInfo.m_spSize.x;

		// 현재 프레임을 인덱스로 변환 후 +1
		int curIndex = AnimUtils::FrameToIndex(m_nowPoint, framePerRow);
		int endIndex = AnimUtils::FrameToIndex(m_aniInfo.endPoint, framePerRow);

		if (m_bhaveBreak && Calc_NowFrame(m_BreakPoint)) return;
		ElapsedTime = 0.f;
		++curIndex;
		if (curIndex > endIndex || curIndex < 0) {
			m_nowPoint = m_aniInfo.isLoop ? m_aniInfo.LoopPoint : m_aniInfo.endPoint;
		}
		else {
			m_nowPoint = AnimUtils::IndexToFrame(curIndex, framePerRow);
		}
		/*if (curIndex > endIndex) {
			if (m_aniInfo.isLoop)
				m_nowPoint = m_aniInfo.LoopPoint;
			else
				m_nowPoint = m_aniInfo.endPoint;
		}
		else {
			m_nowPoint = AnimUtils::IndexToFrame(curIndex, framePerRow);
		}*/
	}

}


void CAnim::Late_Update()
{
	if (m_nowPoint.x == m_aniInfo.endPoint.x &&
		m_nowPoint.y == m_aniInfo.endPoint.y) {
		m_bAnimEnd = true;
	}
	else {
		m_bAnimEnd = false;
	}
}

void CAnim::Render(HDC _hDC)
{
	auto pCam = CCamera::Get_Instance();
	INFO_EX screenInfo;
	// 중심 기준 좌표 + 크기 정보 포함
	screenInfo = pCam->WorldToScreen(m_pOwnObj->Get_Info());


	int drawX = static_cast<int>(screenInfo.fX - screenInfo.offsetLeft);
	int drawY = static_cast<int>(screenInfo.fY - screenInfo.offsetTop);
	int drawW = static_cast<int>(screenInfo.X_Size());
	int drawH = static_cast<int>(screenInfo.Y_Size());

	GdiTransparentBlt(
		_hDC,
		drawX, drawY,
		drawW, drawH,
		memDC,
		m_nowPoint.x * m_aniInfo.m_spSize.x,
		m_nowPoint.y * m_aniInfo.m_spSize.y,
		m_aniInfo.m_spSize.x,
		m_aniInfo.m_spSize.y,
		HOLLOW
	);
	//// 초록색 펜 (테두리용)
	//HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 255));
	//HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);
	//
	//// 내부를 비우기 위해 NULL_BRUSH 사용
	//HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, GetStockObject(NULL_BRUSH));
	//Rectangle(_hDC, drawX, drawY, drawX + drawW, drawY + drawH);
	//// 원래 브러시와 펜으로 복원 및 자원 해제
	//SelectObject(_hDC, hOldBrush);
	//SelectObject(_hDC, hOldPen);
	//DeleteObject(hPen);
}

void CAnim::Release()
{
}

void CAnim::ChangeOnlyPath(const TCHAR* pFilePath)
{
	memDC = CResourceMgr::Get_Instance()->Find_Image(pFilePath);
}

void CAnim::Set_Stop(bool _stop)
{
	m_bStop = _stop;
	if (_stop == false) {
		m_BreakPoint = { 0,0 };
		m_bhaveBreak = false;
	}
}

bool CAnim::Calc_NowFrame(POINT X)
{
	int framePerRow = m_fullTextureSize.cx / m_aniInfo.m_spSize.x;
	int nowIndex = AnimUtils::FrameToIndex(m_nowPoint, framePerRow);
	int PointIndex = AnimUtils::FrameToIndex(X, framePerRow);
	return nowIndex == PointIndex;
}

void CAnim::Set_BreakPoint(POINT X)
{
	m_BreakPoint = X;
	m_bhaveBreak = true;
}

bool CAnim::Less_thanFrame(POINT X)
{
	int framePerRow = m_fullTextureSize.cx / m_aniInfo.m_spSize.x;
	int nowIndex = AnimUtils::FrameToIndex(m_nowPoint, framePerRow);
	int PointIndex = AnimUtils::FrameToIndex(X, framePerRow);
	return PointIndex > nowIndex;
}

// CPP
namespace AnimUtils {

	// 1차원 인덱스를 2D 프레임 좌표로 변환
	POINT IndexToFrame(int index, int framePerRow) {
		POINT pt{};

		if (framePerRow <= 0 || index < 0) {
			// 잘못된 입력 방지용 기본 좌표 반환
			pt.x = 0;
			pt.y = 0;
			return pt;
		}

		pt.x = index % framePerRow;
		pt.y = index / framePerRow;
		return pt;
	}



	// 2D 좌표를 1D 인덱스로 변환
	int FrameToIndex(POINT frame, int framePerRow) {
		return frame.y * framePerRow + frame.x;
	}

	// start~end 사이 프레임 개수 반환
	int FrameRangeToCount(POINT start, POINT end, int framePerRow) {
		int startIndex = FrameToIndex(start, framePerRow);
		int endIndex = FrameToIndex(end, framePerRow);
		return endIndex - startIndex + 1;
	}

	/*SIZE GetBitmapSizeFromDC(HDC hMemDC)
	{
		SIZE size = { 0, 0 };
		HBITMAP hBmp = (HBITMAP)GetCurrentObject(hMemDC, OBJ_BITMAP);

		if (hBmp) {
			BITMAP bmp{};
			GetObject(hBmp, sizeof(BITMAP), &bmp);
			size.cx = bmp.bmWidth;
			size.cy = bmp.bmHeight;
		}
		return size;
	}*/
	SIZE GetBitmapSizeFromDC(HDC hMemDC)
	{
		SIZE size = { 0, 0 };
		HBITMAP hBmp = (HBITMAP)GetCurrentObject(hMemDC, OBJ_BITMAP);

		if (!hBmp) {
			OutputDebugString(L"[오류] DC에 비트맵이 Select되어 있지 않음\n");
			return size;
		}

		BITMAP bmp{};
		GetObject(hBmp, sizeof(BITMAP), &bmp);

		size.cx = bmp.bmWidth;
		size.cy = bmp.bmHeight;
		return size;
	}
}
