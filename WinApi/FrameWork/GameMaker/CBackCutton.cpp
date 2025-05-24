#include "pch.h"
#include "CBackCutton.h"
#include "CResourceMgr.h"
#include "CTimeMgr.h"
CBackCutton::CBackCutton() :m_MovePoint(0.f)
{
	auto pCam = CCamera::Get_Instance();
	prevPoint = pCam->Get_CamPoint();
	m_fMoveSpeed = 1.5f;
	INFO info{ 200,50,100,100 };
	m_tInfo.Translate_Info(info);

	bgW = 400;
	bgH = 300;

}

CBackCutton::~CBackCutton()
{
}

void CBackCutton::Initialize()
{
}

void CBackCutton::Update()
{
	auto pCam = CCamera::Get_Instance();
	POINT cur = pCam->Get_CamPoint();

	int diffX = cur.x - prevPoint.x;

	if (diffX > 0)      m_tVector.fX = -1.f;	  // 카메라 오른쪽 → 배경 왼쪽
	else if (diffX < 0) m_tVector.fX = 1.f;	  //반대
	else                m_tVector.fX = 0.f;

	m_MovePoint += m_tVector.fX * m_fMoveSpeed;
	prevPoint = pCam->Get_CamPoint();


	if ((m_tInfo.fX - m_tInfo.offsetLeft) + m_MovePoint > WINCX) {
		m_tInfo.fX = 0 - m_tInfo.offsetRight;
		m_MovePoint = 0;
	}
	if ((m_tInfo.fX - m_tInfo.offsetRight) + m_MovePoint < 0) {
		m_tInfo.fX = WINCX + m_tInfo.offsetLeft;
		m_MovePoint = 0;
	}
}

void CBackCutton::Late_Update()
{
}

void CBackCutton::Render(HDC _hDC)
{
	TransparentBlt(
		_hDC,
		(m_tInfo.fX - m_tInfo.offsetLeft) + m_MovePoint, 0,
		m_tInfo.X_Size(), 200,
		memDC,
		0, 0,
		bgW, bgH,
		HOLLOW
	);
}

void CBackCutton::Release()
{
}

void CBackCutton::LoadSprite(const TCHAR* ImgKey)
{
	HDC memDC = CResourceMgr::Get_Instance()->Find_Image(ImgKey);
}
