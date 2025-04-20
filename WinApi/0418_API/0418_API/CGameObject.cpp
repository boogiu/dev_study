#include "pch.h"
#include "CGameObject.h"

CGameObject::CGameObject()
	:m_bActive(false)
{
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	ZeroMemory(&m_tRect, sizeof(m_tRect));
}

CGameObject::~CGameObject()
{
}

void CGameObject::Update_Rect()
{
	m_tRect.left = m_tInfo.fX - m_tInfo.fCX;
	m_tRect.right = m_tInfo.fX + m_tInfo.fCX;
	m_tRect.top = m_tInfo.fY - m_tInfo.fCY;
	m_tRect.bottom = m_tInfo.fY + m_tInfo.fCY;
}

INFO CGameObject::GetInfo()
{
	return m_tInfo;
}

void CGameObject::SetINFO(INFO _info)
{
	m_tInfo = _info;
}

void CGameObject::SetPos(float _x, float _y)
{
	m_tInfo.fX = _x;
	m_tInfo.fY = _y;
}

RECT CGameObject::GetRect()
{
	return m_tRect;
}

bool CGameObject::GetActive()
{
	return m_bActive;
}

void CGameObject::SetActive(bool _active)
{
	m_bActive = _active;
}
