#include "pch.h"
#include "CMainGame.h"
#include "CTimeManager.h"
#include "CObjectManager.h"

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
}

void CMainGame::Update()
{
	CTimeManager::Get_Instance().Update();
	CObjectManager::Get_Instance().Update();

}

void CMainGame::Late_Update()
{
	CTimeManager::Get_Instance().Late_Update();
	CObjectManager::Get_Instance().Late_Update();
}

void CMainGame::Render()
{
	Rectangle(m_hBackDC, 0, 0, WINCX, WINCY);
	
	BitBlt(memDC, 0, 0, WINCX, WINCY, m_hBackDC, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	CTimeManager::Get_Instance().Release();
	CObjectManager::Get_Instance().Release();
	ReleaseDC(g_hWnd, memDC); // 윈도우 DC는 해제
}
