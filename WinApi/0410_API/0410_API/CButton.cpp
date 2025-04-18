#include "pch.h"
#include "CButton.h"

CButton::CButton()
	:m_tInfo{0,0,0,0}
{
	ZeroMemory(m_Buffer, lstrlen(m_Buffer));
}

CButton::~CButton()
{
	Release();
}

void CButton::Initialize(const TCHAR* buffer)
{
	m_tInfo.fX = WINCX >> 1;
	m_tInfo.fY = WINCY >> 1;
	m_tInfo.fCX = 200;
	m_tInfo.fCY = 100;
	lstrcpy(m_Buffer, buffer);
}

void CButton::Update()
{
	Update_Rect();
}

void CButton::Render(HDC _hDC)
{
	Rectangle(_hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	DrawText(_hDC, m_Buffer, lstrlen(m_Buffer), &m_tRect, DT_CENTER|DT_VCENTER| DT_SINGLELINE);
}

void CButton::Release()
{
}

void CButton::SetText(const TCHAR* buffer)
{
	lstrcpy(m_Buffer, buffer);
}

void CButton::Update_Rect()
{
	m_tRect.left = m_tInfo.fX - (m_tInfo.fCX * 0.5f);
	m_tRect.top = m_tInfo.fY - (m_tInfo.fCY * 0.5f);
	m_tRect.right = m_tInfo.fX + (m_tInfo.fCX * 0.5f);
	m_tRect.bottom = m_tInfo.fY + (m_tInfo.fCY * 0.5f);
}
