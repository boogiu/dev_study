#include "pch.h"
#include "CTile.h"
#include "CScrollMgr.h"
#include "CResourceMgr.h"
#include "CColider.h"
#include "CUImgr.h"
#include "CCamera.h"

CTile::CTile()
{
	ZeroMemory(&m_tTile, sizeof(TILE_INFO));
}
CTile::CTile(TILE_INFO_LOAD _info)
{
	m_tInfo = _info.m_tInfo;
	m_tRendRC = _info.m_tSpriteRect;
	m_tTile.m_Col = _info.m_Col;
	m_tTile.m_Row = _info.m_Row;

	m_Colider = new CColider(m_tInfo);
}
CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
}

void CTile::Update()
{
	if (m_tTile.m_Col == 0 || m_tTile.m_Col == 3) {
		m_eRole = WALL;
	}
	m_Colider->Update();
}

void CTile::Late_Update()
{
}

void CTile::Render(HDC _hDC)
{
	HDC memDC = CResourceMgr::Get_Instance()->Find_Image(L"Tile");
	if (!memDC) return;

	int srcX = (int)m_tTile.m_Col * CELLSIZE;
	int srcY = (int)m_tTile.m_Row * CELLSIZE;

	INFO_EX screenPos = CCamera::Get_Instance()->WorldToScreen(m_tInfo);
	RECT RenderRect;
	
	screenPos.Update_INFO(RenderRect);

	GdiTransparentBlt(
		_hDC,
		RenderRect.left, RenderRect.top,
		(int)m_tInfo.X_Size(), (int)m_tInfo.Y_Size(),
		memDC,
		srcX, srcY,
		64, 64,
		HOLLOW
	);
	/*GdiTransparentBlt(
		_hDC,
		m_tRendRC.left, m_tRendRC.top,
		(int)m_tInfo.X_Size(), (int)m_tInfo.Y_Size(),
		memDC,
		srcX, srcY,
		64, 64,
		HOLLOW
	);*/
}

void CTile::Release()
{
	Safe_Delete<CColider*>(m_Colider);
}

void CTile::TileSet(TILE_INFO_LOAD _info)
{
	m_tInfo = _info.m_tInfo;
	m_tRendRC = _info.m_tSpriteRect;
	m_tTile.m_Col = _info.m_Col;
	m_tTile.m_Row = _info.m_Row;
	m_Colider = new CColider(m_tInfo);
}
