#include "pch.h"
#include "CMainGame.h"
#include "Managers.h"

CMainGame::CMainGame()
	:m_hDC(nullptr)
{
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	CResourceMgr::Get_Instance()->Initialize();
	CTimeMgr::Get_Instance()->Initialize();
	CKeyMgr::Get_Instance()->Initialize();
	CScrollMgr::Get_Instance()->Initialize();
	CSceneMgr::Get_Instance()->Initialize();
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Back");
}

void CMainGame::Update()
{
	CTimeMgr::Get_Instance()->Update();
	CKeyMgr::Get_Instance()->Update();
	CSceneMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
}

void CMainGame::Render()
{
	HDC hBackDC = CResourceMgr::Get_Instance()->Find_Image(L"Back");
	
	
	CTimeMgr::Get_Instance()->Render(hBackDC);
	CSceneMgr::Get_Instance()->Render(hBackDC);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_hDC);

	CResourceMgr::Destroy_Instance();
	CTimeMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
}
