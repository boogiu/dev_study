#include "pch.h"
#include "CTileMgr.h"
#include "CTile.h"
#include "CScrollMgr.h"
#include "CkeyMgr.h"

#define CELL_SIZE 20.f

CTileMgr* CTileMgr::m_pTileMgr = nullptr;

CTileMgr::CTileMgr()
	:cursorTile(nullptr),m_eDir(LEFT)
{
	//ZeroMemory(&m_Tilelist, sizeof(m_Tilelistr));//오류남
	m_eType = TILE_WALL;
	ZeroMemory(&m_tTile, sizeof(m_tTile));
	ZeroMemory(&m_szBuffer, sizeof(m_szBuffer));
	m_bufferZone = { 10,10, 200, 100 };
	prevPoint = { 0,0 };
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	cursorTile = new CTile();
	cursorTile->Initialize();
}

void CTileMgr::Update()
{
	GetCursorPos(&m_ptMouse);
	ScreenToClient(g_hWnd, &m_ptMouse);

	

	//여기에 스크롤
	if (GetAsyncKeyState(VK_LEFT)) {
		CScrollMgr::Get_Instance()->SetScrollX(20.f);
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		CScrollMgr::Get_Instance()->SetScrollX(-20.f);
	}

	m_ptMouse.x -= CScrollMgr::Get_Instance()->GetScrollX();
	RECT tmpRect{ (m_ptMouse.x / 20) * 20, (m_ptMouse.y / 20) * 20, (m_ptMouse.x / 20 + 1) * 20, (m_ptMouse.y / 20 + 1) * 20 };

	cursorTile->SetRect(tmpRect);
	cursorTile->SetType(m_eType);
	cursorTile->SetDir(m_eDir);


	if (GetAsyncKeyState('0')) {
		m_eType = TILE_NONE;
	}
	if (GetAsyncKeyState('1')) {
		m_eType = TILE_WALL;
	}
	if (GetAsyncKeyState('2')) {
		m_eType = TILE_THORN;
	}
	if (GetAsyncKeyState('3')) {
		m_eType = TILE_INVWALL;
	}
	if (GetAsyncKeyState(VK_DELETE)) {
		Release();
		return;
	}

	if (CkeyMgr::Get_Instance()->isKeyTap('R')) {
		m_eDir = (DIRECTION)((int)m_eDir + 1);
		if (m_eDir == END) {
			m_eDir = LEFT;
		}
	}


	if (CkeyMgr::Get_Instance()->isKeyHold(VK_SHIFT)) {
		DrawDrag(m_ptMouse);
	}
	else {
		DrawOne(m_ptMouse);
	}

	if (GetAsyncKeyState('L')) {
		Load_Data();
	}
	if (GetAsyncKeyState('S')) {
		Save_Data();
	}

	Update_Type();
}

void CTileMgr::Late_Update()
{
}

void CTileMgr::Render(HDC _hDC)
{
	for (int i = 0; i < WINCX / 20; i++) {
		MoveToEx(_hDC, i * 20, 0, nullptr);
		LineTo(_hDC, i * 20, WINCY);
	}

	for (int i = 0; i < WINCY / 20; i++) {
		MoveToEx(_hDC, 0, i * 20, nullptr);
		LineTo(_hDC, WINCX, (i * 20));
	}

	if (!m_Tilelist.empty()) {
		for (CTile* tile : m_Tilelist) {
			tile->Render(_hDC);
		}
	}
	cursorTile->Render(_hDC);
	DrawText(_hDC, m_szBuffer, lstrlen(m_szBuffer), &m_bufferZone, DT_LEFT);
}

void CTileMgr::Release()
{
	for (CTile* tile : m_Tilelist) {
		delete tile;
	}
	m_Tilelist.clear();
}

void CTileMgr::Save_Data()
{
	HANDLE hFile = CreateFile(
		L"../Data/Tile.dat",
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
	for (auto& Tile : m_Tilelist) {
		WriteFile(hFile, &(Tile->GetTile()), sizeof(TILE), &dwByte, nullptr);
	}
	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Save 완료", L"Success", MB_OK);
}

void CTileMgr::Load_Data()
{
	HANDLE hFile = CreateFile(
		L"../Data/Tile.dat",
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
	TILE tTile{};
	DWORD dwByte(0);

	Release();

	while (true) {
		ReadFile(hFile, &tTile, sizeof(TILE), &dwByte, nullptr);

		if (0 == dwByte) break;
		m_Tilelist.push_back(new CTile(tTile));
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Load 완료", L"Success", MB_OK);
}

void CTileMgr::Update_Type()
{
	switch (m_eType) {
	case TILE_NONE:
		swprintf_s(m_szBuffer, L"지우개");
		break;
	case TILE_WALL:
		swprintf_s(m_szBuffer, L"벽");
		break;
	case TILE_THORN:
		swprintf_s(m_szBuffer, L"가시");
		break;
	case TILE_INVWALL:
		swprintf_s(m_szBuffer, L"INVWALL");
		break;
	default:
		swprintf_s(m_szBuffer, L"");
		break;
	}
}


void CTileMgr::DrawOne(POINT& mouse)
{
	if (CkeyMgr::Get_Instance()->isKeyDown(VK_LBUTTON)) { //누르면

		// 최초 클릭 시점
		m_tTile.tRect.left = (mouse.x / 20) * 20;
		m_tTile.tRect.top = (mouse.y / 20) * 20;
		m_tTile.tRect.right = (mouse.x / 20 + 1) * 20;
		m_tTile.tRect.bottom = (mouse.y / 20 + 1) * 20;
		m_tTile.eType = m_eType;
		m_tTile.eDir = m_eDir;

		if (PushTile(m_tTile)) {
			CTile* tile = new CTile(m_tTile);
			tile->Initialize();
			m_Tilelist.push_back(tile);
			ZeroMemory(&m_tTile, sizeof(m_tTile));
		}
	}

}
void CTileMgr::DrawDrag(POINT& mouse)
{
	if (CkeyMgr::Get_Instance()->isKeyTap(VK_LBUTTON)) {
		prevPoint.x = (mouse.x / 20) * 20;
		prevPoint.y = (mouse.y / 20) * 20;
	}

	if (CkeyMgr::Get_Instance()->isKeyAway(VK_LBUTTON)) {
		int startX = min(prevPoint.x, (mouse.x / 20) * 20);
		int startY = min(prevPoint.y, (mouse.y / 20) * 20);

		int endX = max(prevPoint.x, (mouse.x / 20 + 1) * 20);
		int endY = max(prevPoint.y, (mouse.y / 20 + 1) * 20);


		for (int y = startY; y < endY; y += 20) {
			for (int x = startX; x < endX; x += 20) {
				RECT tmpRect = { x, y, x + 20, y + 20 };
				TILE tmpTile = { tmpRect, m_eType,m_eDir };

				if (PushTile(tmpTile)) {
					CTile* tile = new CTile(tmpTile);
					tile->Initialize();
					m_Tilelist.push_back(tile);
				}
			}
		}

		ZeroMemory(&m_tTile, sizeof(m_tTile));
	}
}


bool CTileMgr::PushTile(TILE _tile)
{
	auto iter = find_if(m_Tilelist.begin(), m_Tilelist.end(), [this](CTile* _tile)->bool {
		return (this->Get_Tile() == _tile->GetTile());
		});

	if (iter != m_Tilelist.end()) {
		delete* iter;
		*iter = nullptr;
		m_Tilelist.erase(iter);
	}

	if ((m_eType == TILE_NONE)) {
		return false;
	}

	else {
		return true;
	}

}