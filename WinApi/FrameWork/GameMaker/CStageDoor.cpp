#include "pch.h"
#include "CStageDoor.h"
#include "CResourceMgr.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"

CStageDoor::CStageDoor()
	:m_bOpen(false)
{
	memDC = CResourceMgr::Get_Instance()->Find_Image(L"Stage_Door");
	m_bNext = false;
}

CStageDoor::CStageDoor(TILE_INFO _info)
{
}

CStageDoor::~CStageDoor()
{
}

void CStageDoor::Initialize()
{
	m_bOpen = false;
}

void CStageDoor::Update()
{
	if (CCollisionMgr::Get_Instance()->Trigger_With_Player(this)&&m_bOpen) {
		if (CKeyMgr::Get_Instance()->GetKeyTap('F')) {
			m_bNext = true;
		}
	}
}

void CStageDoor::Late_Update()
{
}

void CStageDoor::Render(HDC _hDC)
{
	POINT point = { 0,0 };
	if (!m_bOpen) {
		point = { 1,1 };
	}
	INFO_EX worldInfo = CCamera::Get_Instance()->WorldToScreen(m_tInfo);
	TransparentBlt(
		_hDC,
		worldInfo.LT().x,
		worldInfo.LT().y,
		worldInfo.X_Size(),
		worldInfo.Y_Size(),
		memDC,
		0, 143*point.x,
		194,143,
		HOLLOW
		);
}

void CStageDoor::Release()
{
}

void CStageDoor::Set_Tile(TILE_INFO _info)
{
	m_tTile.eType = _info.eType;
	m_tTile.tSprite.m_Col = _info.tSprite.m_Col;
	m_tTile.tSprite.m_Row = _info.tSprite.m_Row;
	m_tTile.tInfo.Copy(_info.tInfo);
	m_tInfo.Copy(_info.tInfo);
	m_pCollider = nullptr;
	m_eMapType = STAGE_DOOR;
	INFO_EX BaseInfo = _info.tInfo;
	m_tInfo.fY = BaseInfo.RB().y - 115;
	//m_tInfo.fX =
	m_tInfo.Set_Size(300,250);
}
