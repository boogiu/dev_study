#include "pch.h"
#include "CMainGame.h"
#include "Managers.h"

CMainGame::CMainGame()
	: m_iFrame(0), m_tickCount(GetTickCount64()), m_hDC(nullptr),m_bGameEnd(false)
{
	ZeroMemory(szFrame, lstrlen(szFrame));
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);
	
	if (m_hDC) {
		CInputMgr::GetInstance().Initialize();
		CPhysicsMgr::GetInstance().Initialize();		
		CObjMgr::GetInstance().Initialize();
		CSceneMgr::GetInstance().Initialize();
	}

}

void CMainGame::Update()
{
		CInputMgr::GetInstance().Update();
		CPhysicsMgr::GetInstance().Update();
		CObjMgr::GetInstance().Update();
		CSceneMgr::GetInstance().Update();
		CheckEnd();
}

void CMainGame::Render()
{
	CInputMgr::GetInstance().Render();
	CPhysicsMgr::GetInstance().Render();
	CObjMgr::GetInstance().Render(m_hDC);
	CSceneMgr::GetInstance().Render(m_hDC);

	++m_iFrame;
	RenderFrame();
}

void CMainGame::RenderFrame()
{
	if (m_tickCount + 1000 < GetTickCount64()) {
		swprintf_s(szFrame, L"FPS : %d", m_iFrame);
		SetWindowText(g_hWnd, szFrame);
		m_iFrame = 0;
		m_tickCount = GetTickCount64();
	}
}

void CMainGame::CheckEnd()
{
	if (CSceneMgr::GetInstance().GetNowScene() == SCENE::END) {
		m_bGameEnd = true;
	}
}


void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_hDC);
}
