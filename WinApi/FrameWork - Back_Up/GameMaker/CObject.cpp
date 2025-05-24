#include "pch.h"
#include "CObject.h"
#include "CResourceMgr.h"
#include "CScrollMgr.h"
#include "CColider.h"
CObject::CObject()
	: m_bActive(false), memDC(nullptr), m_Dir(1), m_Colider(nullptr), m_bGround(false)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRendRC, sizeof(RECT));
	ZeroMemory(&m_tVector, sizeof(VECTOR2));
}

CObject::~CObject()
{
}

void CObject::OnCollisionEnter(CObject* object, RECT collRECT)
{
}

void CObject::Update_RECT()
{
	m_tInfo.Update_INFO(m_tRendRC);
}

void CObject::Render_BMP(const TCHAR* pImageKey, HDC _hDC,POINT index)
{
	//int scrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//int scrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//memDC = CResourceMgr::Get_Instance()->Find_Image(pImageKey);
	//if (!memDC) return;

	//GdiTransparentBlt(
	//	_hDC,
	//	m_tRendRC.left + scrollX,
	//	m_tRendRC.top + scrollX,
	//	(int)m_tInfo.fCX, (int)m_tInfo.fCY,
	//	memDC,
	//	0+(index.x * m_tInfo.fCX), 0+(index.y * m_tInfo.fCY),
	//	(int)m_tInfo.fCX, (int)m_tInfo.fCY,
	//	HOLLOW
	//);
}
