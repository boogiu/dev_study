#include "pch.h"
#include "CMainGame.h"
#include "CTimeManager.h"
#include "CObjectManager.h"
#include "CRenderManager.h"
#include "CKeyManager.h"
#include "CPlayer.h"

CMainGame::CMainGame()
	:memDC(nullptr),m_hBackBitmap(nullptr),m_hBackDC(nullptr)
{
}

CMainGame::~CMainGame()
{
}
void CMainGame::Initialize()
{
	memDC = GetDC(g_hWnd);
	m_hBackDC = CreateCompatibleDC(memDC);
	m_hBackBitmap = CreateCompatibleBitmap(memDC, WINCX, WINCY);
	m_hOldBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

	CTimeManager::Get_Instance().Initialize();
	CObjectManager::Get_Instance().Initialize();
	CRenderManager::Get_Instance().Initialize();
	CKeyManager::Get_Instance().Initialize();

	m_pPlayer= CObjectManager::Get_Instance().Create_Object<CPlayer>();
	m_pPlayer->Initialize();
}

void CMainGame::Update()
{
	CTimeManager::Get_Instance().Update();
	CObjectManager::Get_Instance().Update();
	CRenderManager::Get_Instance().Update();
	CKeyManager::Get_Instance().Update();
}

void CMainGame::Late_Update()
{
	CTimeManager::Get_Instance().Late_Update();
	CObjectManager::Get_Instance().Late_Update();
	CRenderManager::Get_Instance().Late_Update();
	CKeyManager::Get_Instance().Late_Update();
}

void CMainGame::Render()
{
	Rectangle(m_hBackDC, 0, 0, WINCX, WINCY);
	CRenderManager::Get_Instance().Render(m_hBackDC);
	BitBlt(memDC, 0, 0, WINCX, WINCY, m_hBackDC, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	CTimeManager::Get_Instance().Release();
	CObjectManager::Get_Instance().Release();
	CKeyManager::Get_Instance().Release();
	ReleaseDC(g_hWnd, memDC); // 윈도우 DC는 해제
}
