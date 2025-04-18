#include "pch.h"
#include "CMainGame.h"
#include "CInputMgr.h"
#include "CSceneMgr.h"


CMainGame::CMainGame()
	:m_hDC(nullptr)
{
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Intialize()
{
	m_hDC = GetDC(g_hWnd);
	CInputMgr::GetInstance().Initialize();
	CSceneMgr::GetInstance().Initialize();
}

void CMainGame::Update()
{
	CInputMgr::GetInstance().Update();
	CSceneMgr::GetInstance().Update();
}

void CMainGame::Late_Update()
{
	CInputMgr::GetInstance().Late_Update();
	CSceneMgr::GetInstance().Late_Update();
}

void CMainGame::Render()
{
	CInputMgr::GetInstance().Render();
	CSceneMgr::GetInstance().Render(m_hDC);
}

void CMainGame::Release()
{
	Safe_Delete<CObject*>(m_pPlayer);
}
