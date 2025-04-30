#include "pch.h"
#include "CMain.h"
#include "CTileMgr.h"
#include "CScrollMgr.h"
#include "CkeyMgr.h"
CMain::CMain()
	:m_hDC(nullptr)
{
}

CMain::~CMain()
{
	Release();
}

void CMain::Initialize()
{
	m_hDC = GetDC(g_hWnd);
	CTileMgr::Get_Instance()->Initialize();
}

void CMain::Update()
{
	CTileMgr::Get_Instance()->Update();
	CkeyMgr::Get_Instance()->Update();
}

void CMain::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
}
void CMain::Render()
{
	RECT tmp;
	GetClientRect(g_hWnd, &tmp);

	// 1. 메모리 DC 생성
	HDC hMemDC = CreateCompatibleDC(m_hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(m_hDC, tmp.right, tmp.bottom);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	// 2. 메모리 DC 배경 지우기
	FillRect(hMemDC, &tmp, (HBRUSH)GetStockObject(WHITE_BRUSH)); // 흰 배경 (또는 COLOR_BACKGROUND)

	// 3. 메모리 DC에 전체 그리기
	CTileMgr::Get_Instance()->Render(hMemDC); // 여기서 hMemDC로 그림

	// 4. 메모리 DC -> 실제 DC 복사
	BitBlt(m_hDC, 0, 0, tmp.right, tmp.bottom, hMemDC, 0, 0, SRCCOPY);

	// 5. 정리
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
}


void CMain::Release()
{
	CTileMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CkeyMgr::Destroy_Instance();
}
