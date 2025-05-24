#include "pch.h"
#include "CStage.h"
#include "CResourceMgr.h"
#include "CScrollMgr.h"
#include "CObject.h"
#include "CTile.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"

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
	TILE_INFO_LOAD tTile{};
	DWORD dwByte(0);
	m_TileList.clear();

	while (true) {
		ReadFile(hFile, &tTile, sizeof(TILE_INFO_LOAD), &dwByte, nullptr);

		if (0 == dwByte) break;
		CObject* tmp = CObjMgr::Get_Instance()->ReQuestObject<CTile>(PLATFORM);
		dynamic_cast<CTile*>(tmp)->TileSet(tTile);
		m_TileList.push_back(tmp);
		
	}
	CloseHandle(hFile);
}
