#include "pch.h"
#include "CSpawnPoint.h"
#include "CObjMgr.h"
#include "Objects.h"
#include "CFunction.h"

CSpawnPoint::CSpawnPoint() :myEnemy(nullptr)
{
}

CSpawnPoint::CSpawnPoint(TILE_INFO _info)
	:myEnemy(nullptr)
{
}

CSpawnPoint::~CSpawnPoint()
{
	Release();
}

void CSpawnPoint::Initialize()
{
	m_bSpawn = false;
}

void CSpawnPoint::Update()
{

	if (myEnemy) {
		if (!myEnemy->Is_Activate()) {
			m_bActive = false;
		}
	}
}

void CSpawnPoint::Late_Update()
{
	if (!m_bSpawn) {
		m_tInfo.Update_INFO(m_tRect);
		if (!CCamera::Get_Instance()->Out_of_Camera(m_tRect)) {
			Spawn();
		}
	}
}

void CSpawnPoint::Render(HDC _hDC)
{
	//INFO_EX info = CCamera::Get_Instance()->WorldToScreen(m_tInfo);
	//Rectangle(_hDC, info.LT().x, info.LT().y, info.RB().x, info.RB().y);
}

void CSpawnPoint::Release()
{
}

void CSpawnPoint::Set_Tile(TILE_INFO _info)
{
	m_tTile.eType = _info.eType;
	m_tTile.tSprite.m_Col = _info.tSprite.m_Col;
	m_tTile.tSprite.m_Row = _info.tSprite.m_Row;
	m_tTile.tInfo.Copy(_info.tInfo);
	m_tInfo.Copy(_info.tInfo);
	m_pCollider = nullptr;
	m_eMapType = SPAWN_POINT;
}

void CSpawnPoint::Spawn()
{
	int Rand = CFunction::GetInt(0, 3);
	switch (Rand)
	{
	case 0:
		myEnemy = CObjMgr::Get_Instance()->ReQuestObject<CSoldier>(MONSTER);
		break;
	case 1:
		myEnemy = CObjMgr::Get_Instance()->ReQuestObject<CArchor>(MONSTER);
		break;
	case 2:
		myEnemy = CObjMgr::Get_Instance()->ReQuestObject<CMage>(MONSTER);
		break;
	case 3:
		myEnemy = CObjMgr::Get_Instance()->ReQuestObject<CGiantSoldier>(MONSTER);
		break;
	default:
		break;
	}
	myEnemy->Initialize();
	float distance = myEnemy->Get_Collider()->Get_Info().offsetBottom;
	myEnemy->Set_Pos(m_tInfo.fX, m_tInfo.LT().y - distance);
	m_bSpawn = true;
}


