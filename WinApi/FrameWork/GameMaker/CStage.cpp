#include "pch.h"
#include "CStage.h"
#include "CResourceMgr.h"
#include "CScrollMgr.h"

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
	DWORD dwByte(0);
	m_TileList.clear();

	while (true) {
		ReadFile(hFile, &tTile, sizeof(TILE_INFO), &dwByte, nullptr);

		if (0 == dwByte) break;
		m_TileList.push_back(tTile);
	}
	CloseHandle(hFile);
	//MessageBox(g_hWnd, L"Load ¿Ï·á", L"Success", MB_OK);
}

void CStage::Render_Tile(HDC _hDC)
{

	float x = CScrollMgr::Get_Instance()->Get_ScrollX();
	float y = CScrollMgr::Get_Instance()->Get_ScrollY();

	for (TILE_INFO& info : m_TileList) {
		HDC memDC = CResourceMgr::Get_Instance()->Find_Image(L"Platform");
		if (!memDC) return;

		int srcX = (int)info.m_Col * CELLSIZE;
		int srcY = (int)info.m_Row * CELLSIZE;

		Rectangle(_hDC, info.m_tSpriteRect.left, info.m_tSpriteRect.top, info.m_tSpriteRect.right, info.m_tSpriteRect.bottom);
	}
}
