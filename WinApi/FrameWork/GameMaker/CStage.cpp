#include "pch.h"
#include "CStage.h"
#include "CResourceMgr.h"
#include "CScrollMgr.h"
#include "CObject.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include "Objects.h"
#include "CInfiniteObj.h"

CStage::CStage()
{
}

CStage::~CStage()
{
}

void CStage::Initialize()
{
}

void CStage::Update()
{
}

void CStage::Late_Update()
{
}

void CStage::Render(HDC _hDC)
{

}

void CStage::Release()
{
}

void CStage::Load_Map(const TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(
		pFilePath,
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (INVALID_HANDLE_VALUE == hFile) {
		MessageBox(g_hWnd, L"Load Error", L"Error", MB_OK);
		return;
	}

	TILE_INFO tTile{};
	DWORD dwByte = 0;
	m_StageTile.clear();

	while (true) {
		// 각 Read마다 성공 여부 확인
		if (!ReadFile(hFile, &tTile, sizeof(TILE_INFO), &dwByte, nullptr) || dwByte == 0)
			break;
		CObject* tmp = nullptr;

		if (tTile.eType == TILE_OBJ) {
			tmp = Set_ObjType(tTile);
		}
		else {
			tmp = CObjMgr::Get_Instance()->ReQuestObject<CTile>(PLATFORM);
		}

		if (tmp)
		{
			CTile* ptile = static_cast<CTile*>(tmp);
			ptile->Set_Tile(tTile);
			m_StageTile[tTile.eType].push_back(tmp);
			switch (ptile->Get_Obj_Type())
			{
			case START_POINT:
				m_StartPoint = ptile;
				break;
			case SPAWN_POINT:
				m_SpawnContainer.push_back(ptile);
				break;
			case STAGE_DOOR:
				m_StageDoor = ptile;
				break;
			case REWARD_BOX:
				m_RewardBox = ptile;
				break;		
			case TRAP_OBJ:
				break;
			case NOT_MAPOBJ:
				break;
			case MAX_MAPOBJ:
				break;
			default:
				break;
			}
		}
	}

	//std::sort(m_SpawnContainer.begin(), m_SpawnContainer.end(),
	//	[&](CObject* a, CObject* b)
	//	{
	//		float dx1 = a->Get_Info().fX - m_StartPoint->Get_Info().fX;
	//		float dy1 = a->Get_Info().fY - m_StartPoint->Get_Info().fY;
	//		float dx2 = b->Get_Info().fX - m_StartPoint->Get_Info().fX;
	//		float dy2 = b->Get_Info().fY - m_StartPoint->Get_Info().fY;
	//
	//		float dist1 = dx1 * dx1 + dy1 * dy1;
	//		float dist2 = dx2 * dx2 + dy2 * dy2;
	//
	//		return dist1 < dist2;
	//	});

	CloseHandle(hFile);
	//MessageBox(g_hWnd, L"Load 완료", L"Success", MB_OK);
}
void CStage::Render_BackObj(HDC _hDC)
{
	for (auto& obj : m_BackStage) {
		obj->Render(_hDC);
	}
	for (auto& obj : m_BackBridge) {
		obj->Render(_hDC);
	}

	for (auto& obj : m_BackCutton) {
		obj->Render(_hDC);
	}
}
void CStage::Update_BackObj()
{
	for (auto& obj : m_BackStage) {
		obj->Update();
	}
	for (auto& obj : m_BackBridge) {
		obj->Update();
	}

	for (auto& obj : m_BackCutton) {
		obj->Update();
	}
}
void CStage::Set_Player_Pos()
{
	CObject* player = CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER);
	player->Get_Info().fX = m_StartPoint->Get_Info().fX;
	player->Get_Info().fY = m_StartPoint->Get_Info().fY;
}
CObject* CStage::Set_ObjType(TILE_INFO tile)
{
	if (tile.eType != TILE_OBJ) return nullptr;
	CObject* instance = nullptr;
	if (tile.tSprite.m_Col == 0 && tile.tSprite.m_Row == 0) {
		instance = CObjMgr::Get_Instance()->ReQuestObject<CStartPoint>(PLATFORM);
	}
	else if (tile.tSprite.m_Col == 0 && tile.tSprite.m_Row == 1) {
		instance = CObjMgr::Get_Instance()->ReQuestObject<CSpawnPoint>(PLATFORM);
	}
	else if (tile.tSprite.m_Col == 1 && tile.tSprite.m_Row == 0) {
		instance = CObjMgr::Get_Instance()->ReQuestObject<CStageDoor>(PLATFORM);
	}
	else if (tile.tSprite.m_Col == 1 && tile.tSprite.m_Row == 1) {
		instance = CObjMgr::Get_Instance()->ReQuestObject<CRewardBox>(PLATFORM);
	}

	return instance;
}
