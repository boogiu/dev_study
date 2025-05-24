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
	CSceneMgr::Get_Instance()->Initialize();
	CObjMgr::Get_Instance()->Initialize();	
	CCollisionMgr::Get_Instance()->Initialize();
	CUIMgr::Get_Instance()->Initialize();
	CCamera::Get_Instance()->Initialize();
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/BackGround/Defualt_Back.bmp", L"backBuffer");
}

void CMainGame::Update()
{
	CTimeMgr::Get_Instance()->Update();
	CKeyMgr::Get_Instance()->Update();
	CSceneMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();
	CCollisionMgr::Get_Instance()->Update();
	CUIMgr::Get_Instance()->Update();
	CCamera::Get_Instance()->Update();

}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update();
	CCollisionMgr::Get_Instance()->Late_Update();
	CUIMgr::Get_Instance()->Late_Update();
	CCamera::Get_Instance()->Late_Update();

}

void CMainGame::Render()
{
	HDC hBackDC = CResourceMgr::Get_Instance()->Find_Image(L"backBuffer");
	
	
	CSceneMgr::Get_Instance()->Render(hBackDC);
	CObjMgr::Get_Instance()->Render(hBackDC);
	CTimeMgr::Get_Instance()->Render(hBackDC);
	CCollisionMgr::Get_Instance()->Render(hBackDC);
	CUIMgr::Get_Instance()->Render(hBackDC);
	CCamera::Get_Instance()->Render(hBackDC);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_hDC);

	CResourceMgr::Destroy_Instance();
	CTimeMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	CCollisionMgr::Destroy_Instance();
	CUIMgr::Destroy_Instance();
	CCamera::Destroy_Instance();

}
