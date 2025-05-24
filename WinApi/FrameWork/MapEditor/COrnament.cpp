#include "pch.h"
#include "COrnament.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

COrnament::COrnament()
{
	m_tInfo.Set_FileKey(L"Ornament");
	memDC = CBmpMgr::Get_Instance()->Find_Image(m_tInfo.tSprite.fileKey);
}

COrnament::COrnament(int Col, int Row)
{
	m_tInfo.Set_FileKey(L"Ornament");
	memDC = CBmpMgr::Get_Instance()->Find_Image(m_tInfo.tSprite.fileKey);
	m_tInfo.tSprite.m_Col = Col;
	m_tInfo.tSprite.m_Row = Row;
}

COrnament::COrnament(TILE_INFO _info)
{
	m_tInfo.eType = TILE_ORNAM;
	m_tInfo.tSprite.m_Col = _info.tSprite.m_Col;
	m_tInfo.tSprite.m_Row = _info.tSprite.m_Row;
	m_tInfo.Set_FileKey(_info.tSprite.fileKey);
	m_tInfo.tInfo.Copy(_info.tInfo);
	memDC = CBmpMgr::Get_Instance()->Find_Image(m_tInfo.tSprite.fileKey);

}

COrnament::~COrnament()
{
}

void COrnament::Initialize()
{
	
}

void COrnament::Update()
{
}

void COrnament::Late_Update()
{
}

void COrnament::Render(HDC _hDC)
{
	float x = CScrollMgr::Get_Instance()->Get_ScrollX();
	float y = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_bStick) {
		x = 0;
		y = 0;
	}
	float LEFT = m_tInfo.tInfo.fX - m_tInfo.tInfo.offsetLeft;
	float TOP = m_tInfo.tInfo.fY - m_tInfo.tInfo.offsetTop;

	TransparentBlt(
		_hDC,
		LEFT + x,
		TOP + y,
		m_tInfo.tInfo.X_Size(),
		m_tInfo.tInfo.Y_Size(),
		memDC,
		m_tInfo.tSprite.m_Col * CELLSIZE,
		m_tInfo.tSprite.m_Row * CELLSIZE,
		CELLSIZE, CELLSIZE,
		HOLLOW
	);
}

void COrnament::Release()
{
}
