#include "pch.h"
#include "../MapEditor/CMapObj.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CMapObj::CMapObj()
{
}
CMapObj::CMapObj(TILE_INFO _info)
{
	m_tInfo.eType = TILE_OBJ;
	m_tInfo.tSprite.m_Col = _info.tSprite.m_Col;
	m_tInfo.tSprite.m_Row = _info.tSprite.m_Row;
	m_tInfo.tInfo.Copy(_info.tInfo);
}

CMapObj::CMapObj(int Col, int Row)
{
	memDC = CBmpMgr::Get_Instance()->Find_Image(m_tInfo.tSprite.fileKey);
	m_tInfo.tSprite.m_Col = Col;
	m_tInfo.tSprite.m_Row = Row;
}
CMapObj::~CMapObj()
{
}

void CMapObj::Initialize()
{
}

void CMapObj::Update()
{
}

void CMapObj::Late_Update()
{
}
void CMapObj::Render(HDC _hDC)
{
	float x = CScrollMgr::Get_Instance()->Get_ScrollX();
	float y = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_bStick) {
		x = 0;
		y = 0;
	}

	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, GetStockObject(NULL_BRUSH));

	MapObj ColRow = { m_tInfo.tSprite.m_Col ,m_tInfo.tSprite.m_Row };
	HPEN hPen = nullptr;

	if (ColRow == MapObj(0, 0))       hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	else if (ColRow == MapObj(0, 1))  hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	else if (ColRow == MapObj(1, 0))  hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
	else if (ColRow == MapObj(1, 1))  hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 255));

	HPEN hOldPen = nullptr;
	if (hPen)
	{
		hOldPen = (HPEN)SelectObject(_hDC, hPen);
	}

	RECT tmpRect;
	m_tInfo.tInfo.Update_INFO(tmpRect);

	Rectangle(
		_hDC,
		tmpRect.left + x,
		tmpRect.top + y,
		tmpRect.right + x,
		tmpRect.bottom + y
	);

	if (hPen)
	{
		SelectObject(_hDC, hOldPen);
		DeleteObject(hPen);
	}

	SelectObject(_hDC, hOldBrush);
}


void CMapObj::Release()
{
}
