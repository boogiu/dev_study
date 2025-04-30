#include "pch.h"
#include "CTileMgr.h"
#include "CTile.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
	:m_nowTile(nullptr), ptMouse{}, m_bGrid(false), m_bUI(true)
{
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/tile_32_4.bmp", L"Tile");


	INFO StartPoint = { 30,30 , CL_CELLSIZE>>1,CL_CELLSIZE>>1 };
	INFO DrawPoint = StartPoint;

	for (int i = 0; i < 20; ++i) { //타일 타입 같은 행
		for (int j = 0; j < 5; ++j) { //타일 방향  같은 열
			CTile* tmpTile = new CTile(DrawPoint,i, j);
			tmpTile->Initialize();
			tmpTile->Set_IsMove(false);
			m_tileUIContainer.push_back(tmpTile);
			DrawPoint.fX += DrawPoint.fCX + 2;
		}
		if (DrawPoint.fY > WINCY -30) {
			DrawPoint.fY = 30;
			StartPoint.fX += 230;
		}
		else {
			DrawPoint.fY += DrawPoint.fCY + 2;
		}
		DrawPoint.fX = StartPoint.fX;
	}
	INFO m_nowTileRect = { 0,0 , CL_CELLSIZE>>2,CL_CELLSIZE>>2 };
	m_nowTile = new CTile(m_nowTileRect, 0, 0);
	m_nowTile->Initialize();
	m_nowTile->Set_IsMove(false);
}

void CTileMgr::Update()
{
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	LONG x = (LONG)CScrollMgr::Get_Instance()->Get_ScrollX();
	LONG y = (LONG)CScrollMgr::Get_Instance()->Get_ScrollY();

	m_nowTile->Set_Pos({
		ptMouse.x,
		ptMouse.y });

	ptMouse.x -= x;
	ptMouse.y -= y;


	Key_Check();
	if (m_bUI) {
		for (CTile* tile : m_tileUIContainer) {
			tile->Update();
		}
	}
	
	for (CTile* tile : m_tileDataContainer)
	{
		tile->Update();
	}
	m_nowTile->Update();
}

void CTileMgr::Late_Update()
{

}

void CTileMgr::Render(HDC _hDC)
{
	HDC hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground");
	BitBlt(_hDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	if (m_bGrid) {
		for (int i = 0; i < WINCX; i += CL_CELLSIZE) {
			for (int j = 0; j < WINCY; j += CL_CELLSIZE) {
				MoveToEx(_hDC, i, j, nullptr);
				LineTo(_hDC, i, WINCY);
				MoveToEx(_hDC, i, j, nullptr);
				LineTo(_hDC, WINCX, j);
			}
		}
	}

	if (m_bUI) {
		for (CTile* tile : m_tileUIContainer) {
			tile->Render(_hDC);
		}
	}



	for (CTile* tile : m_tileDataContainer)
	{
		tile->Render(_hDC);
	}
	m_nowTile->Render(_hDC);

}

void CTileMgr::Release()
{
	delete m_nowTile;
	m_nowTile = nullptr;
	for (CTile*& tile : m_tileDataContainer) {
		delete tile;
		tile = nullptr;
	}
	m_tileDataContainer.clear();

	for (CTile*& tile : m_tileUIContainer) {
		delete tile;
		tile = nullptr;
	}
	m_tileUIContainer.clear();
}

void CTileMgr::Key_Check()
{
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_RIGHT)) {
		CScrollMgr::Get_Instance()->Set_ScrollX(-CELLSIZE);
	}
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_LEFT)) {
		CScrollMgr::Get_Instance()->Set_ScrollX(CELLSIZE);
	}
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_UP)) {
		CScrollMgr::Get_Instance()->Set_ScrollY(CELLSIZE);
	}
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_DOWN)) {
		CScrollMgr::Get_Instance()->Set_ScrollY(-CELLSIZE);
	}
	if (CKeyMgr::Get_Instance()->GetKeyTap('G')) {
		m_bGrid = !m_bGrid;
	}
	if (CKeyMgr::Get_Instance()->GetKeyTap('B')) {
		m_bUI = !m_bUI;
	}
	if (CKeyMgr::Get_Instance()->GetKeyTap('L')) {
		Load_Tile();
	}	if (CKeyMgr::Get_Instance()->GetKeyTap('S')) {
		Save_Tile();
	}
	if (CKeyMgr::Get_Instance()->GetKeyTap(VK_DELETE)) {
		for (CTile*& tile : m_tileDataContainer) {
			delete tile;
			tile = nullptr;
		}
		m_tileDataContainer.clear();
	}

	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_LBUTTON)) {
		if (m_bUI) {
			auto iter = find_if(m_tileUIContainer.begin(), m_tileUIContainer.end(), [this](CTile* tile)->bool {
				RECT tile2 = tile->GetRect();
				return PtInRect(&tile2, ptMouse);
				});
			if (iter == m_tileUIContainer.end()) {
				Push_Tile();
			}
			else {
				Change_Tile(m_nowTile, *iter, false, false);
			}
		}
		else {
			Push_Tile();
		}
	}
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_RBUTTON)) {
		Erase_Tile();
	}
}

void CTileMgr::Push_Tile()
{
	auto iter = find_if(m_tileDataContainer.begin(), m_tileDataContainer.end(), [this](CTile* tile)->bool {
		RECT tile2 = tile->GetRect();
		return PtInRect(&tile2, ptMouse);
		});

	if (iter == m_tileDataContainer.end()) {

		TILE_INFO info = m_nowTile->Get_TileINFO();
		info.m_tInfo.fCX = CL_CELLSIZE;
		info.m_tInfo.fCY = CL_CELLSIZE;
		info.m_tInfo.fX = ((ptMouse.x / CL_CELLSIZE) * CL_CELLSIZE) + (CL_CELLSIZE >> 1);
		info.m_tInfo.fY = ((ptMouse.y / CL_CELLSIZE) * CL_CELLSIZE) + (CL_CELLSIZE >> 1);

		CTile* newTile = new CTile(info.m_tInfo, info.m_Row, info.m_Col);
		newTile->Initialize();
		m_tileDataContainer.push_back(newTile);
	}
	else {
		Change_Tile(*iter, m_nowTile, false, false);
	}
}

void CTileMgr::Change_Tile(CTile* Dst, CTile* Src, bool Resize, bool Repos)
{
	TILE_INFO tmp = Src->Get_TileINFO();
	Dst->Set_Col(tmp.m_Col);
	Dst->Set_Row(tmp.m_Row);
	if (Resize) {
		Dst->Set_SCALE({ (LONG)tmp.m_tInfo.fCX,(LONG)tmp.m_tInfo.fCY });
	}
	if (Repos) {
		Dst->Set_Pos({ (LONG)tmp.m_tInfo.fX,(LONG)tmp.m_tInfo.fY });
	}
}

void CTileMgr::Erase_Tile()
{
	auto iter = find_if(m_tileDataContainer.begin(), m_tileDataContainer.end(), [this](CTile* tile)->bool {
		RECT tile2 = tile->GetRect();
		return PtInRect(&tile2, ptMouse);
		});
	if (iter != m_tileDataContainer.end()) {
		delete (*iter);
		(*iter) = nullptr;
		m_tileDataContainer.erase(iter);
	}
}

void CTileMgr::Save_Tile()
{
	HANDLE hFile = CreateFile(
		L"../TileData/Tile.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (INVALID_HANDLE_VALUE == hFile) {
		MessageBox(g_hWnd, L"Save Error", L"Error", MB_OK);
		return;
	}

	DWORD dwByte(0);

	if (m_tileDataContainer.empty()) return;

	for (auto& Tile : m_tileDataContainer) {
		TILE_INFO tmp = Tile->Get_TileINFO();
		WriteFile(hFile, &tmp, sizeof(TILE_INFO), &dwByte, nullptr);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Save 완료", L"Success", MB_OK);
}

void CTileMgr::Load_Tile()
{
	HANDLE hFile = CreateFile(
		L"../TileData/Tile.dat",
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
	for (CTile*& tile : m_tileDataContainer) {
		delete tile;
		tile = nullptr;
	}
	m_tileDataContainer.clear();

	while (true) {
		ReadFile(hFile, &tTile, sizeof(TILE_INFO), &dwByte, nullptr);

		if (0 == dwByte) break;
		m_tileDataContainer.push_back(new CTile(tTile));
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Load 완료", L"Success", MB_OK);
}
