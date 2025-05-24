#include "pch.h"
#include "CTileMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "../MapEditor/CTile.h"
#include "COrnament.h"
#include "CMapObj.h"
#include "CPlatforms.h"


CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
	: ptWorld{}, m_bGrid(false), m_bUI(true), m_iPltType(0)
{
	//m_Filename = L"TutorMap";
	m_Filename = L"FirstMap";
	//m_Filename = L"BossStage";
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackGround/Defualt_Back.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackGround/Defualt_Back.bmp", L"Defualt_Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackGround/Boss_Back.bmp", L"Boss_Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/tile_32_4.bmp", L"Tile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/tile_32_4.bmp", L"Platform");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/BackOrnament.bmp", L"Ornament");
	BufferTypeRect = { 50,0, 300, 50 };
	m_enowType = TILE_PLATFORM;


	m_pNowTile[TILE_PLATFORM] = new CPlatforms;
	m_pNowTile[TILE_OBJ] = new CMapObj;
	m_pNowTile[TILE_ORNAM] = new COrnament;

	for (int i = 0; i < TILE_TYPE::TILE_END; ++i) {
		m_TileContainer.insert({ (TILE_TYPE)i, std::vector<CTile*>() });
		m_pNowTile[(TILE_TYPE)i]->UI_Set();
		m_pNowTile[(TILE_TYPE)i]->Set_Stick(true);
	}

	//플랫폼 타일
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 4; ++j) {
			CTile* tmp = new CPlatforms(j, i);
			tmp->UI_Set();
			tmp->Set_Stick(true);
			m_TileContainer[TILE_PLATFORM].push_back(tmp);
		}
	}
	//장식품 타일
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			CTile* tmp = new COrnament(j, i);
			tmp->UI_Set();
			tmp->Set_Stick(true);
			m_TileContainer[TILE_ORNAM].push_back(tmp);
		}
	}
	//맵 오브젝트
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			CTile* tmp = new CMapObj(j, i);
			tmp->UI_Set();
			tmp->Set_Stick(true);
			m_TileContainer[TILE_OBJ].push_back(tmp);
		}
	}
}

void CTileMgr::Update()
{
	GetCursorPos(&ptWorld); //->월드 좌표
	ScreenToClient(g_hWnd, &ptWorld);

	GetCursorPos(&ptScreen); //->스크린 좌표
	ScreenToClient(g_hWnd, &ptScreen);

	LONG x = (LONG)CScrollMgr::Get_Instance()->Get_ScrollX();
	LONG y = (LONG)CScrollMgr::Get_Instance()->Get_ScrollY();

	ptWorld.x -= x;
	ptWorld.y -= y;

	swprintf_s(szPosBuffer, L"X: %d Y: %d /X: %d Y: %d /  ", ptWorld.x, ptWorld.y, ptScreen.x, ptScreen.y);

	if (m_pNowTile[m_enowType]) {
		m_pNowTile[m_enowType]->Get_Info().fX = ptScreen.x;
		m_pNowTile[m_enowType]->Get_Info().fY = ptScreen.y;
	}

	BufferPosRect = { ptScreen.x - 150, ptScreen.y - 40, ptScreen.x + 150, ptScreen.y };

	Key_Check();

}

void CTileMgr::Late_Update()
{

}

void CTileMgr::Render(HDC _hDC)
{
	HDC hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Defualt_Back");

	LONG x = (LONG)CScrollMgr::Get_Instance()->Get_ScrollX();
	LONG y = (LONG)CScrollMgr::Get_Instance()->Get_ScrollY();

	//BitBlt(_hDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);
	TransparentBlt(_hDC, 0, 0, WINCX, WINCY, hGroundDC, 150, 0, 600, 400, HOLLOW);

	for (int i = 0; i < TILE_TYPE::TILE_END; ++i) {
		for (CTile*& tile : m_TileDataContainer[(TILE_TYPE)i]) {
			tile->Render(_hDC);
		}
	}

	SetTextColor(_hDC, RGB(255, 255, 255));
	SetBkMode(_hDC, TRANSPARENT);
	DrawText(_hDC, szPosBuffer, lstrlen(szPosBuffer), &BufferPosRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if (m_pNowTile[m_enowType]) {
		m_pNowTile[m_enowType]->Render(_hDC);
	}

	Render_TileType(_hDC);

	if (m_bUI) {
		Render_UI_Tile(_hDC);
	}


	if (m_bGrid) {
		for (int i = 0; i < MAP_CX; i += CL_CELLSIZE) {
			MoveToEx(_hDC, i, 0, nullptr);
			LineTo(_hDC, i, MAP_CY);
		}
		for (int i = 0; i < MAP_CY; i += CL_CELLSIZE) {
			MoveToEx(_hDC, 0, i, nullptr);
			LineTo(_hDC, MAP_CX, i);
		}
	}

	MoveToEx(_hDC, 0 + x, 0, nullptr);
	LineTo(_hDC, 0 + x, MAP_CY);

	MoveToEx(_hDC, WINCX + x, 0, nullptr);
	LineTo(_hDC, WINCX + x, MAP_CY);
}

void CTileMgr::Release()
{

}

void CTileMgr::Key_Check()
{
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_RIGHT)) {
		CScrollMgr::Get_Instance()->Set_ScrollX(-32);
	}
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_LEFT)) {
		CScrollMgr::Get_Instance()->Set_ScrollX(32);
	}
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_UP)) {
		CScrollMgr::Get_Instance()->Set_ScrollY(32);
	}
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_DOWN)) {
		CScrollMgr::Get_Instance()->Set_ScrollY(-32);
	}

	if (CKeyMgr::Get_Instance()->GetKeyTap('G')) {
		m_bGrid = !m_bGrid;
	}
	if (CKeyMgr::Get_Instance()->GetKeyTap('B')) {
		m_bUI = !m_bUI;
	}

	if (CKeyMgr::Get_Instance()->GetKeyTap('1')) {//TILE_PLATFORM, TILE_OBJ, TILE_ORNAM
		m_enowType = (TILE_TYPE)0;
	}
	if (CKeyMgr::Get_Instance()->GetKeyTap('2')) {
		m_enowType = (TILE_TYPE)1;
	}
	if (CKeyMgr::Get_Instance()->GetKeyTap('3')) {
		m_enowType = (TILE_TYPE)2;
	}

	if (CKeyMgr::Get_Instance()->GetKeyTap(VK_F1)) {//TILE_PLATFORM, TILE_OBJ, TILE_ORNAM
		m_iPltType = 0;
	}
	if (CKeyMgr::Get_Instance()->GetKeyTap(VK_F2)) {
		m_iPltType = 1;
	}
	if (CKeyMgr::Get_Instance()->GetKeyTap(VK_F3)) {
		m_iPltType = 2;
	}

	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_LBUTTON)) {
		if (m_bUI) {
			auto iter = find_if(m_TileContainer[m_enowType].begin(), m_TileContainer[m_enowType].end(),
				[this](CTile* tile) -> bool {
					RECT tileRect = tile->Get_RECT();
					return PtInRect(&tileRect, this->Get_ScreenPT());
				}
			);

			if (iter != m_TileContainer[m_enowType].end()) {
				m_pNowTile[m_enowType]->Get_Sprite().m_Col = (*iter)->Get_Sprite().m_Col;
				m_pNowTile[m_enowType]->Get_Sprite().m_Row = (*iter)->Get_Sprite().m_Row;
			}
			else {
				Push_Tile();
			}
		}
		else {
			Push_Tile();
		}
	}
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_RBUTTON)) {
		Erase_Tile();
	}
	if (CKeyMgr::Get_Instance()->GetKeyTap(VK_DELETE)) {
		Erase_All(m_enowType);
	}

	if (CKeyMgr::Get_Instance()->GetKeyTap('L')) {
		Load_Tile();
	}	if (CKeyMgr::Get_Instance()->GetKeyTap('S')) {
		Save_Tile();
	}
}

void CTileMgr::Push_Tile()
{
	auto iter = find_if(m_TileDataContainer[m_enowType].begin(), m_TileDataContainer[m_enowType].end(),
		[this](CTile* tile) -> bool {
			RECT tileRect = tile->Get_RECT();
			return PtInRect(&tileRect, this->Get_WorldPT());
		});
	//타일이 같은 위치에 있다면 스프라이트만 교체
	if (iter != m_TileDataContainer[m_enowType].end()) {
		(*iter)->Get_Sprite().m_Col = m_pNowTile[m_enowType]->Get_Sprite().m_Col;
		(*iter)->Get_Sprite().m_Row = m_pNowTile[m_enowType]->Get_Sprite().m_Row;
	}
	//타일이 같은 위치에 없다면 추가
	else {
		CTile* tile = nullptr;
		INFO tile_Info = { 0,0,0,0 };
		switch (m_enowType)
		{
		case TILE_PLATFORM:
		{
			tile = new CPlatforms;
			tile_Info = {
				(float)(ptWorld.x / CL_CELLSIZE * CL_CELLSIZE + CL_CELLSIZE * 0.5f),
				(float)(ptWorld.y / CL_CELLSIZE * CL_CELLSIZE + CL_CELLSIZE * 0.5f),
				CL_CELLSIZE,
				CL_CELLSIZE };

			int snapX = (ptWorld.x / CL_CELLSIZE);
			if (ptWorld.x < 0) snapX -= 1; // 음수 보정
			tile_Info.fX = snapX * CL_CELLSIZE + CL_CELLSIZE * 0.5f;
		}
		break;
		case TILE_OBJ:
			{
				tile = new CMapObj;
				tile_Info = {
					(float)(ptWorld.x / CL_CELLSIZE * CL_CELLSIZE + CL_CELLSIZE * 0.5f),
					(float)(ptWorld.y / CL_CELLSIZE * CL_CELLSIZE + CL_CELLSIZE * 0.5f),
					CL_CELLSIZE,
					CL_CELLSIZE };

				int snapX = (ptWorld.x / CL_CELLSIZE);
				if (ptWorld.x < 0) snapX -= 1; // 음수 보정
				tile_Info.fX = snapX * CL_CELLSIZE + CL_CELLSIZE * 0.5f;
			}
			break;
		case TILE_ORNAM:
		{
			tile = new COrnament;
			tile_Info = {
				(float)(ptWorld.x / CL_CELLSIZE * CL_CELLSIZE + CL_CELLSIZE * 0.5f),
				(float)(ptWorld.y / CL_CELLSIZE * CL_CELLSIZE + CL_CELLSIZE * 0.5f),
				CL_CELLSIZE,
				CL_CELLSIZE };

			int snapX = (ptWorld.x / CL_CELLSIZE);
			if (ptWorld.x < 0) snapX -= 1; // 음수 보정
			tile_Info.fX = snapX * CL_CELLSIZE + CL_CELLSIZE * 0.5f;
		}
		break;
		case TILE_END:
			break;
		default:
			break;
		}
		if (tile) {
			tile->Get_Sprite().m_Col = m_pNowTile[m_enowType]->Get_Sprite().m_Col;
			tile->Get_Sprite().m_Row = m_pNowTile[m_enowType]->Get_Sprite().m_Row;
			tile->Get_Info().Translate_Info(tile_Info);
			m_TileDataContainer[m_enowType].push_back(tile);
		}
	}
}


void CTileMgr::Erase_Tile()
{
	auto iter = find_if(m_TileDataContainer[m_enowType].begin(), m_TileDataContainer[m_enowType].end(),
		[this](CTile* tile) -> bool {
			RECT tileRect = tile->Get_RECT();
			return PtInRect(&tileRect, this->Get_WorldPT());
		});
	//타일이 같은 위치에 있다면 타일 삭제
	if (iter != m_TileDataContainer[m_enowType].end()) {
		delete* iter;
		m_TileDataContainer[m_enowType].erase(iter);
	}
}

void CTileMgr::Save_Tile()
{
	wstring filePath = L"../TileData/" + m_Filename + L".dat";
	HANDLE hFile = CreateFile(
		filePath.c_str(),
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
	TILE_INFO tTile{};
	if (m_TileDataContainer.empty()) return;

	for (int i = 0; i < TILE_TYPE::TILE_END; ++i) {
		for (CTile*& tile : m_TileDataContainer[(TILE_TYPE)i]) {
			tTile.tInfo = tile->Get_Info();
			tTile.tSprite = tile->Get_Sprite();
			tTile.eType = (TILE_TYPE)i;
			WriteFile(hFile, &tTile, sizeof(TILE_INFO), &dwByte, nullptr);
		}
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Save 완료", L"Success", MB_OK);
}
void CTileMgr::Load_Tile()
{
	wstring filePath = L"../TileData/" + m_Filename + L".dat";

	HANDLE hFile = CreateFile(
		filePath.c_str(),
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
	Erase_All(TILE_END);
	m_TileDataContainer.clear();

	while (true) {
		// 각 Read마다 성공 여부 확인
		if (!ReadFile(hFile, &tTile, sizeof(TILE_INFO), &dwByte, nullptr) || dwByte == 0)
			break;

		CTile* tmp = nullptr;
		if (tTile.tInfo.X_Size() == 0) continue;
		switch (tTile.eType) {
		case TILE_PLATFORM: tmp = new CPlatforms(tTile); break;
		case TILE_ORNAM:    tmp = new COrnament(tTile); break;
		case TILE_OBJ:      tmp = new CMapObj(tTile); break;
		default: break;
		}

		if (tmp)
			m_TileDataContainer[tTile.eType].push_back(tmp);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Load 완료", L"Success", MB_OK);
}

void CTileMgr::Render_TileType(HDC _hDC)
{
	switch (m_enowType)
	{
	case TILE_PLATFORM:
		swprintf_s(szTypeBuffer, L"TYPE : 플랫폼 타일 ");
		break;
	case TILE_OBJ:
		swprintf_s(szTypeBuffer, L"TYPE : 맵 오브젝트 ");
		break;
	case TILE_ORNAM:
		swprintf_s(szTypeBuffer, L"TYPE : 장식품 ");
		break;
	case TILE_END:
		break;
	default:
		break;
	}
	DrawText(_hDC, szTypeBuffer, lstrlen(szTypeBuffer), &BufferTypeRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

}

void CTileMgr::Render_UI_Tile(HDC _hDC)
{
	for (CTile* tile : m_TileContainer[m_enowType]) {
		tile->Render(_hDC);
	}
}

void CTileMgr::Erase_All(TILE_TYPE type)
{
	if (type != TILE_TYPE::TILE_END) {
		for (CTile*& tile : m_TileDataContainer[m_enowType]) {
			delete tile;
		}
		m_TileDataContainer[m_enowType].clear();
	}
	else {
		for (int i = 0; i < (int)TILE_END; ++i) {
			for (CTile*& tile : m_TileDataContainer[(TILE_TYPE)i]) {
				delete tile;
			}
			m_TileDataContainer[(TILE_TYPE)i].clear();
		}
	}
}
