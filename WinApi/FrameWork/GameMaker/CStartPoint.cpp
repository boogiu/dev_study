#include "pch.h"
#include "CStartPoint.h"

CStartPoint::CStartPoint()
{
}

CStartPoint::CStartPoint(TILE_INFO _info)
{
}

CStartPoint::~CStartPoint()
{
}

void CStartPoint::Set_Tile(TILE_INFO _info)
{
	m_tTile.eType = _info.eType;
	m_tTile.tSprite.m_Col = _info.tSprite.m_Col;
	m_tTile.tSprite.m_Row = _info.tSprite.m_Row;
	m_tTile.tInfo.Copy(_info.tInfo);
	m_tInfo.Copy(_info.tInfo);
	m_pCollider = nullptr;
	m_eMapType = START_POINT;
}
