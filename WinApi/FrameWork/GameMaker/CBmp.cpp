#include "pch.h"
#include "CBmp.h"

CBmp::CBmp()
	: m_hBitMap(nullptr),m_hOldMap(nullptr),m_hMemDC(nullptr)
{
}

CBmp::~CBmp()
{
	Release();
}

void CBmp::Initialize()
{
}

void CBmp::Update()
{
}

void CBmp::Late_Update()
{
}

void CBmp::Render(HDC _hDC)
{
}

void CBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldMap);
	DeleteObject(m_hBitMap);
	DeleteDC(m_hMemDC);
}

void CBmp::Load_Bmp(const TCHAR* pFilePath)
{
	HDC hDC = GetDC(g_hWnd);
	m_hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(g_hWnd,hDC);

	m_hBitMap = (HBITMAP)LoadImage(
		NULL,
		pFilePath,
		IMAGE_BITMAP,
		0,0,
		LR_LOADFROMFILE|LR_CREATEDIBSECTION
	);

	m_hOldMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);
}
