#include "pch.h"
#include "CResourceMgr.h"
#include "CCollider.h"
#include "../GameMaker/CTile.h"

CTile::CTile()
{
}
CTile::CTile(TILE_INFO _info)
{
	m_tTile.eType = _info.eType;
	m_tTile.tSprite.m_Col = _info.tSprite.m_Col;
	m_tTile.tSprite.m_Row = _info.tSprite.m_Row;
	m_tTile.Set_FileKey(_info.tSprite.fileKey);
	m_tTile.tInfo.Copy(_info.tInfo);
	m_tInfo.Copy(_info.tInfo);
	memDC = CResourceMgr::Get_Instance()->Find_Image(_info.tSprite.fileKey);
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
	m_tTile.tInfo.Update_INFO(m_tRect);
	if (m_pCollider) {
		m_pCollider->Update();
		m_pCollider->Update_Center(m_tTile.tInfo.fX, m_tTile.tInfo.fY);
	}
}

void CTile::Late_Update()
{
}

void CTile::Render(HDC _hDC)
{
	INFO_EX wordlInfo = CCamera::Get_Instance()->WorldToScreen(m_tTile.tInfo);

	TransparentBlt(
		_hDC,
		wordlInfo.fX- wordlInfo.offsetLeft,
		wordlInfo.fY - wordlInfo.offsetTop,
		wordlInfo.X_Size(),
		wordlInfo.Y_Size(),
		memDC,
		m_tTile.tSprite.m_Col * CELLSIZE,
		m_tTile.tSprite.m_Row * CELLSIZE,
		CELLSIZE, CELLSIZE,
		HOLLOW
	);


}

void CTile::Release()
{
	Safe_Delete<CCollider*>(m_pCollider);
}

void CTile::Set_Tile(TILE_INFO _info)
{
	m_tTile.eType = _info.eType;
	m_tTile.tSprite.m_Col = _info.tSprite.m_Col;
	m_tTile.tSprite.m_Row = _info.tSprite.m_Row;
	m_tTile.Set_FileKey(_info.tSprite.fileKey);
	m_tTile.tInfo.Copy(_info.tInfo);
	m_tInfo.Copy(_info.tInfo);
	memDC= CResourceMgr::Get_Instance()->Find_Image(m_tTile.tSprite.fileKey);
	if (m_tTile.eType == TILE_PLATFORM) {
		m_pCollider = new CCollider(this);
		m_pCollider->Get_Info().Copy(m_tTile.tInfo);
	}
	else {
		m_pCollider = nullptr;
	}
	m_eMapType = NOT_MAPOBJ;
}
