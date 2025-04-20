#include "pch.h"
#include "CMainGame.h"
#include "Managers.h"

CMainGame::CMainGame()
	:m_bGameEnd(false),m_hDC(nullptr)
{
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	if(!m_hDC)
	m_hDC = GetDC(g_hWnd);

	CSceneMgr::Get_Instance()->Intialize();
	CObjMgr::Get_Instance()->Intialize();
	CInputMgr::Get_Instance()->Initialize();
}

void CMainGame::Update()
{
	CSceneMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();
	CInputMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update();
	CInputMgr::Get_Instance()->Late_Update();
}

void CMainGame::Render()
{
	CSceneMgr::Get_Instance()->Render(m_hDC);
	CObjMgr::Get_Instance()->Render(m_hDC);
	CInputMgr::Get_Instance()->Render(m_hDC);
}

void CMainGame::Release()
{
	CSceneMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	CInputMgr::Destroy_Instance();
}
