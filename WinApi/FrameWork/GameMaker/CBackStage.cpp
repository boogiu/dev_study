#include "pch.h"
#include "CBackStage.h"
#include "CResourceMgr.h"
#include "CTimeMgr.h"
#include "CBackCutton.h"

CBackStage::CBackStage():m_MovePoint(0.f)
{
	auto pCam = CCamera::Get_Instance();
	m_fMoveSpeed = 0.8f;
	bgW = 320;
	bgH = 400;
	prevPoint = pCam->Get_CamPoint();
}

CBackStage::~CBackStage()
{
	Release();
}

void CBackStage::Initialize()
{
	
}
void CBackStage::Update()
{
	auto pCam = CCamera::Get_Instance();
	POINT cur = pCam->Get_CamPoint();

	int diffX = cur.x - prevPoint.x;

	if (diffX > 0)      m_tVector.fX = -1.f;  // 카메라 오른쪽 → 배경 왼쪽
	else if (diffX < 0) m_tVector.fX = 1.f;
	else                m_tVector.fX = 0.f;

	m_MovePoint += m_fMoveSpeed * m_tVector.fX;
	prevPoint = cur;

	if (m_MovePoint >= bgW) m_MovePoint += bgW;
	else if (m_MovePoint < 0)   m_MovePoint -= bgW;

}

void CBackStage::Late_Update()
{
}

void CBackStage::Render(HDC _hDC)
{
	HDC memDC = CResourceMgr::Get_Instance()->Find_Image(L"BackWall");


	// 이동량 wrap-around
	int offset = -(int)m_MovePoint % bgW;
	if (offset < 0) offset -= bgW;
	if (Check({ -offset, 0,bgW, 200 })) {
		// 첫 번째 이미지
		TransparentBlt(
			_hDC,
			-offset, 0,
			bgW, WINCY,
			memDC,
			0, 0,
			bgW, bgH,
			HOLLOW
		);

	}

	if (Check({ -bgW - offset, 0,bgW, 200 })) {
		// 두 번째 이미지
		TransparentBlt(
			_hDC,
			bgW - offset, 0,
			bgW, WINCY,
			memDC,
			bgW, 0,
			bgW, bgH,
			HOLLOW
		);
	}

	if (Check({ bgW * 2 - offset, 0,bgW, 200 })) {
		// 세 번째 이미지
		TransparentBlt(
			_hDC,
			bgW * 2 - offset, 0,
			bgW, WINCY,
			memDC,
			bgW*2, 0,
			bgW, bgH,
			HOLLOW
		);
	}
	if (Check({ bgW * 3 - offset, 0,bgW, 200 })) {
		// 세 번째 이미지
		TransparentBlt(
			_hDC,
			bgW * 3 - offset, 0,
			bgW, WINCY,
			memDC,
			bgW * 3, 0,
			bgW, bgH,
			HOLLOW
		);
	}
}

void CBackStage::Release()
{
}

void CBackStage::LoadSprite(const TCHAR* ImgKey)
{

}

bool CBackStage::Check(RECT tRect)
{
	if (tRect.right < 0 || tRect.left > WINCX ||
		tRect.bottom < 0 || tRect.top > WINCY)
	{
		return false;
	}

	return true;
}

