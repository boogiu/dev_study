#include "pch.h"
#include "CTile.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
CTile::CTile()
	:m_bMove(true)
{
	ZeroMemory(&m_tTile, sizeof(TILE_INFO));
}
CTile::CTile(INFO _INFO, int  Row, int  Col)
	:m_bMove(true)
{
	m_tTile.m_tInfo = _INFO;
	m_tTile.m_Row = Row;
	m_tTile.m_Col = Col;
}
CTile::~CTile()
{
}

void CTile::Initialize()
{
}

void CTile::Update()
{
	Update_Rect();
}

void CTile::Late_Update()
{
}

void CTile::Render(HDC _hDC)
{
	float x;
	float y;

	if (m_bMove) {
		x = CScrollMgr::Get_Instance()->Get_ScrollX();
		y = CScrollMgr::Get_Instance()->Get_ScrollY();
	}
	else {
		x = 0;
		y = 0;
	}


	HDC memDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile");
	if (!memDC) return;

	int srcX = (int)m_tTile.m_Col * CELLSIZE;
	int srcY = (int)m_tTile.m_Row * CELLSIZE;

	GdiTransparentBlt(
		_hDC,
		m_tTile.m_tSpriteRect.left + x, m_tTile.m_tSpriteRect.top + y,
		(int)m_tTile.m_tInfo.fCX, (int)m_tTile.m_tInfo.fCY,
		memDC,
		srcX, srcY,
		64,64,
		HOLLOW
	);
}

void CTile::Release()
{
}

void CTile::Update_Rect()
{
	m_tTile.m_tSpriteRect.left = static_cast<LONG>(m_tTile.m_tInfo.fX - (m_tTile.m_tInfo.fCX * 0.5f));
	m_tTile.m_tSpriteRect.right = static_cast<LONG>(m_tTile.m_tInfo.fX + (m_tTile.m_tInfo.fCX * 0.5f));
	m_tTile.m_tSpriteRect.top = static_cast<LONG>(m_tTile.m_tInfo.fY - (m_tTile.m_tInfo.fCY * 0.5f));
	m_tTile.m_tSpriteRect.bottom = static_cast<LONG>(m_tTile.m_tInfo.fY + (m_tTile.m_tInfo.fCY * 0.5f));
}
