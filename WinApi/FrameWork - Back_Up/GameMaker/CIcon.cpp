#include "pch.h"
#include "CIcon.h"
#include "CResourceMgr.h"

CIcon::CIcon()
{
}

CIcon::~CIcon()
{
}

void CIcon::Initialize()
{
}

void CIcon::Update()
{
	m_tInfo.Update_INFO(m_tRect);
}

void CIcon::Late_Update()
{
}

void CIcon::Render(HDC _hDC)
{
	GdiTransparentBlt(
		_hDC,
		m_tRect.left,
		m_tRect.top,
		m_tInfo.X_Size(), m_tInfo.Y_Size(),
		memDC,
		0, 0,
		SpriteWidth, SpriteHeight,
		HOLLOW
	);
}

void CIcon::Release()
{
}

void CIcon::LoadSprite(const TCHAR* pImageKey)
{
	memDC = CResourceMgr::Get_Instance()->Find_Image(pImageKey);
	BITMAP bmp = {};
	HBITMAP hBmp = (HBITMAP)GetCurrentObject(memDC, OBJ_BITMAP);
	if (hBmp) {
		GetObject(hBmp, sizeof(BITMAP), &bmp);
		SpriteWidth = bmp.bmWidth;
		SpriteHeight= bmp.bmHeight;
	}
}
