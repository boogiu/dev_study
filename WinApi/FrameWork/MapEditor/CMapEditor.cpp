#include "pch.h"
#include "CMapEditor.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CBmpMgr.h"

CMapEditor::CMapEditor()
:m_hDC(nullptr)
{
}

CMapEditor::~CMapEditor()
{
	Release();
}

void CMapEditor::Initialize()
{
	m_hDC = GetDC(g_hWnd);
	CBmpMgr::Get_Instance()->Initialize();
	CKeyMgr::Get_Instance()->Initialize();
	CScrollMgr::Get_Instance()->Initialize();
	CTileMgr::Get_Instance()->Initialize();
	

}

void CMapEditor::Update()
{
	CKeyMgr::Get_Instance()->Update();
	CTileMgr::Get_Instance()->Update();
}

void CMapEditor::Late_Update()
{
	CKeyMgr::Get_Instance()->Late_Update();
	CTileMgr::Get_Instance()->Late_Update();
	CScrollMgr::Get_Instance()->Scroll_Lock();

}

void CMapEditor::Render()
{

	HDC hBackDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");
	
	CTileMgr::Get_Instance()->Render(hBackDC);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);
}

void CMapEditor::Release()
{
	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	ReleaseDC(g_hWnd, m_hDC);
}
