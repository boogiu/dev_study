#include "pch.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CResourceMgr.h"
#include "CObject.h"
#include "CUImgr.h"
#include"CCamera.h"

CAnim::~CAnim()
{
}

void CAnim::Set_Anim(const TCHAR* pFilePath,ANI_FRAME anim)
{
	memDC = CResourceMgr::Get_Instance()->Find_Image(pFilePath);
	m_aniInfo = anim;
	m_nowPoint = m_aniInfo.startPoint;
	ElapsedTime = 0;
}

void CAnim::Update()
{
	ElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();

	if (ElapsedTime > m_aniInfo.frameSpeed)
	{
		m_nowPoint.x += 1;
		ElapsedTime = 0.f;

		// x 초과 시
		if (m_nowPoint.x > m_aniInfo.endPoint.x)
		{
			m_nowPoint.x = 0;
			m_nowPoint.y += 1;

			// y도 초과 시
			if (m_nowPoint.y > m_aniInfo.endPoint.y)
			{
				if (m_aniInfo.isLoop)
					m_nowPoint = m_aniInfo.LoopPoint;
				else
					m_nowPoint = m_aniInfo.endPoint;
			}
		}
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

	// 중심 기준 좌표 + 크기 정보 포함
	INFO_EX screenInfo = pCam->WorldToScreen(m_pOwner->Get_Info());

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
}

void CAnim::Release()
{
}

void CAnim::ChangeOnlyPath(const TCHAR* pFilePath)
{
	memDC = CResourceMgr::Get_Instance()->Find_Image(pFilePath);
}
