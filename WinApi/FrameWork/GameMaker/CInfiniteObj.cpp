#include "pch.h"
#include "CInfiniteObj.h"
#include "CResourceMgr.h"
#include "CTimeMgr.h"
#include "CCamera.h"

CInfiniteObj::CInfiniteObj()
	: m_fMoveSpeed(0.f), m_fMovePointX(0.f), m_tInfo{}, m_tVector{}, m_prevPoint{}
	, memDC(nullptr), m_pCam(nullptr), m_iSpHeight(0), m_iSpWidth(0),m_iIndex(0),m_bIsOut(false), m_fMovePointY(0.f)
{
}

CInfiniteObj::~CInfiniteObj()
{
}

void CInfiniteObj::Initialize()
{
	m_pCam = CCamera::Get_Instance();
	m_prevPoint = m_pCam->Get_CamPoint();
}

void CInfiniteObj::Update()
{
	POINT cur = m_pCam->Get_CamPoint();
	//m_pCam->Out_of_Camera
	int diffX = cur.x - m_prevPoint.x;
	int diffY= cur.y - m_prevPoint.y;

	if (diffX > 0)      m_tVector.fX = -1.f;	  // 카메라 오른쪽 → 배경 왼쪽
	else if (diffX < 0) m_tVector.fX = 1.f;	  //반대
	else                m_tVector.fX = 0.f;	
	
	
	if (diffY > 0)      m_tVector.fY = -1.f;	  // 카메라 오른쪽 → 배경 왼쪽
	else if (diffY < 0) m_tVector.fY = 1.f;	  //반대
	else                m_tVector.fY = 0.f;

	m_fMovePointX += (m_tVector.fX * m_fMoveSpeed);
	m_fMovePointY += (m_tVector.fY * m_fMoveSpeed*0.12f);

	m_prevPoint = m_pCam->Get_CamPoint();


	float leftX = m_tInfo.fX - m_tInfo.offsetLeft + (m_fMovePointX);
	float rightX = m_tInfo.fX + m_tInfo.offsetRight + (m_fMovePointX);

	
	if (leftX > WINCX) {
		m_tInfo.fX =0 - m_tInfo.offsetRight;  
		m_fMovePointX = 0;
		m_bIsOut = true;
	}
	else if (rightX < 0) {
		m_tInfo.fX = WINCX+ m_tInfo.offsetLeft;
		m_fMovePointX = 0;
		m_bIsOut = true;
	}else{
		m_bIsOut = false;
	}
}

void CInfiniteObj::Late_Update()
{
}

void CInfiniteObj::Render(HDC _hDC)
{
	

	// 화면 밖이면 렌더링 스킵 (컬링)
	if (m_bIsOut)
		return;

	TransparentBlt(
		_hDC,
		(m_tInfo.fX - m_tInfo.offsetLeft) + (m_fMovePointX), (m_tInfo.fY - m_tInfo.offsetTop)+ m_fMovePointY,
		m_tInfo.X_Size(), m_tInfo.Y_Size(),
		memDC,
		m_tInfo.X_Size()*m_iIndex, 0,
		m_tInfo.X_Size(), m_iSpHeight,
		HOLLOW
	);

	//int drawX = (int)(m_tInfo.fX - m_tInfo.offsetLeft + m_fMovePoint);
	//int drawY = 0;
	//int drawW = m_tInfo.X_Size();
	//int drawH = m_tInfo.Y_Size();
	//// 펜 생성 (빨간색, 1px)
	//
	// hPen = CreateSolidBrush(RGB(255, 60*m_iIndex, 0));
	//HBRUSH hOldPen = (HBRUSH)SelectObject(_hDC, hPen);

	//// 테두리 그리기
	//Rectangle(_hDC, drawX, drawY, drawX + drawW, drawY + drawH);

	//// 펜 정리
	//SelectObject(_hDC, hOldPen);
	//DeleteObject(hPen);
}

void CInfiniteObj::Release()
{
}

void CInfiniteObj::LoadSprite(const TCHAR* ImgKey)
{
	memDC = CResourceMgr::Get_Instance()->Find_Image(ImgKey);
}
