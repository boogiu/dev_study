#include "pch.h"
#include "../GameMaker/CMapObj.h"
#include "CRader.h"

CMapObj::CMapObj()
{
}
CMapObj::CMapObj(TILE_INFO _info)
{
	m_tTile.eType = _info.eType;
	m_tTile.tSprite.m_Col = _info.tSprite.m_Col;
	m_tTile.tSprite.m_Row = _info.tSprite.m_Row;
	m_tTile.tInfo.Copy(_info.tInfo);
	m_tInfo.Copy(_info.tInfo);
	Set_ObjType();
}

CMapObj::~CMapObj()
{
	Release();
}

void CMapObj::Initialize()
{
	Set_ObjType();
}

void CMapObj::Update()
{
	m_tTile.tInfo.Update_INFO(m_tRect);
	if (m_pCollider) {
		m_pCollider->Update();
		m_pCollider->Update_Center(m_tTile.tInfo.fX, m_tTile.tInfo.fY);
	}
}

void CMapObj::Late_Update()
{
}
void CMapObj::Render(HDC _hDC)
{
	INFO_EX wordlInfo = CCamera::Get_Instance()->WorldToScreen(m_tTile.tInfo);
	
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, GetStockObject(NULL_BRUSH));
	HPEN hPen = nullptr;

	switch (m_eObjType)
	{
	case START_POINT:
		//hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
		break;
	case SPAWN_POINT:
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		break;
	case STAGE_DOOR:
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
		break;
	case TRAP_OBJ:
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 255));
		break;
	case MAX_MAPOBJ:
		break;
	default:
		break;
	}
	HPEN hOldPen = nullptr;
	if (hPen)
	{
		hOldPen = (HPEN)SelectObject(_hDC, hPen);
	}

	Rectangle(
		_hDC,
		wordlInfo.fX - wordlInfo.offsetLeft,
		wordlInfo.fY - wordlInfo.offsetTop,
		wordlInfo.fX + wordlInfo.offsetRight,
		wordlInfo.fY + wordlInfo.offsetRight
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

void CMapObj::Set_Tile(TILE_INFO _info)
{
	m_tTile.eType = _info.eType;
	m_tTile.tSprite.m_Col = _info.tSprite.m_Col;
	m_tTile.tSprite.m_Row = _info.tSprite.m_Row;
	//m_tTile.Set_FileKey(_info.tSprite.fileKey);
	m_tTile.tInfo.Copy(_info.tInfo);
	m_tInfo.Copy(_info.tInfo);
	m_pCollider = nullptr;
	Set_ObjType();
}

void CMapObj::Set_ObjType()
{
	if (m_tTile.tSprite.m_Col == 0 && m_tTile.tSprite.m_Row == 0) {
		m_eObjType = START_POINT;
	}
	else if (m_tTile.tSprite.m_Col == 0 && m_tTile.tSprite.m_Row == 1) {
		m_eObjType = SPAWN_POINT;
	}
	else if (m_tTile.tSprite.m_Col == 1 && m_tTile.tSprite.m_Row == 0) {
		m_eObjType = STAGE_DOOR;
	}
	else if (m_tTile.tSprite.m_Col == 1 && m_tTile.tSprite.m_Row == 1) {
		m_eObjType = TRAP_OBJ;
	}
}
