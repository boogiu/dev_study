#include "pch.h"
#include "CObject.h"
#include "CResourceMgr.h"
#include "CScrollMgr.h"
CObject::CObject()
	: m_bActive(false),memDC(nullptr)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRendRC, sizeof(RECT));
	ZeroMemory(&m_tCollRC, sizeof(RECT));
	ZeroMemory(&m_tVector, sizeof(VECTOR2));
}

CObject::~CObject()
{
}

void CObject::Update_RECT()
{
	m_tRendRC.left = static_cast<LONG>(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRendRC.right = static_cast<LONG>(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRendRC.top = static_cast<LONG>(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRendRC.bottom = static_cast<LONG>(m_tInfo.fY + (m_tInfo.fCY * 0.5f));

	m_tCollRC.left = static_cast<LONG>(m_tInfo.fX - (m_tInfo.fCX * 0.2f));
	m_tCollRC.right = static_cast<LONG>(m_tInfo.fX + (m_tInfo.fCX * 0.2f));
	m_tCollRC.top = static_cast<LONG>(m_tInfo.fY - (m_tInfo.fCY * 0.2f));
	m_tCollRC.bottom = static_cast<LONG>(m_tInfo.fY + (m_tInfo.fCY * 0.2f));
	
}

void CObject::Render_BMP(const TCHAR* pImageKey, HDC _hDC,POINT index)
{
	int scrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int scrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	memDC = CResourceMgr::Get_Instance()->Find_Image(pImageKey);
	if (!memDC) return;

	GdiTransparentBlt(
		_hDC,
		m_tRendRC.left + scrollX,
		m_tRendRC.top + scrollX,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY,
		memDC,
		0+(index.x * m_tInfo.fCX), 0+(index.y * m_tInfo.fCY),
		(int)m_tInfo.fCX, (int)m_tInfo.fCY,
		HOLLOW
	);
}
