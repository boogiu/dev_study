#include "pch.h"
#include "CTile.h"
#include "CScrollMgr.h"

CTile::CTile()
{
	ZeroMemory(&m_tTile, sizeof(m_tTile));
}

CTile::CTile(TILE _info)
{
	ZeroMemory(&m_tTile, sizeof(m_tTile));
	m_tTile.tRect = _info.tRect;
	m_tTile.eType = _info.eType;
    m_tTile.eDir = _info.eDir;
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
}

void CTile::Late_Update()
{
}
void CTile::Render(HDC _hDC)
{

    COLORREF color = RGB(0, 0, 0); // 기본: 검정색


    switch (m_tTile.eType)
    {
    case TILE_WALL:
        color = RGB(0, 0, 0); // 검정
        break;
    case TILE_THORN:
        color = RGB(255, 0, 0); // 빨강
        break;
    case TILE_INVWALL:
        color = RGB(0, 255, 0); // 초록
        break;
    case TILE_END:
    default:
        break;
    }

    POINT tmpPoint;
    
    switch (m_tTile.eDir)
    {
    case LEFT:
        tmpPoint.x = m_tTile.tRect.left;
        tmpPoint.y = (m_tTile.tRect.bottom + m_tTile.tRect.top)*0.5f;
        break;
    case RIGHT:
        tmpPoint.x = m_tTile.tRect.right;
        tmpPoint.y = (m_tTile.tRect.bottom + m_tTile.tRect.top) * 0.5f;
        break;
    case UP:
        tmpPoint.x = (m_tTile.tRect.right + m_tTile.tRect.left) * 0.5f;
        tmpPoint.y = m_tTile.tRect.top;
        break;

    case DOWN:
        tmpPoint.x = (m_tTile.tRect.right + m_tTile.tRect.left) * 0.5f;
        tmpPoint.y = m_tTile.tRect.bottom;
        break;
    default:
        tmpPoint.x = m_tTile.tRect.left;
        tmpPoint.y = (m_tTile.tRect.bottom + m_tTile.tRect.top) * 0.5f;
        break;
    }

    HBRUSH nowBrush = CreateSolidBrush(color);
    HBRUSH originBrush = (HBRUSH)SelectObject(_hDC, nowBrush);

    Rectangle(_hDC, 
        m_tTile.tRect.left + CScrollMgr::Get_Instance()->GetScrollX(),
        m_tTile.tRect.top, 
        m_tTile.tRect.right + CScrollMgr::Get_Instance()->GetScrollX(),
        m_tTile.tRect.bottom);

    SelectObject(_hDC, originBrush);
    DeleteObject(nowBrush);
   
   HBRUSH newBrush = CreateSolidBrush(RGB(189,189,189));
   HBRUSH neworigin = (HBRUSH)SelectObject(_hDC, newBrush);
  
    Rectangle(_hDC,
        tmpPoint.x - 3,
        tmpPoint.y - 3,
        tmpPoint.x + 3,
        tmpPoint.y + 3);

    SelectObject(_hDC, originBrush);
    DeleteObject(newBrush);
}


void CTile::Release()
{
}

void CTile::SetRect(RECT _rc)
{
    this->m_tTile.tRect = _rc;
}

void CTile::SetType(TILE_ID _ID)
{
    this->m_tTile.eType = _ID;
}

void CTile::SetDir(DIRECTION _dir)
{
    this->m_tTile.eDir = _dir;
}
