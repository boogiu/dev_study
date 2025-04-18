#include "pch.h"
#include "CTile.h"

CTile::CTile()
	:m_tInfo{0,0,0,0},m_tRect{0,0,0,0}
{
}

CTile::~CTile()
{
}

void CTile::Update_Rect()
{
	m_tRect.left = m_tInfo.fX - (m_tInfo.fCX * 0.5f);
	m_tRect.top = m_tInfo.fY - (m_tInfo.fCY * 0.5f);
	m_tRect.right = m_tInfo.fX + (m_tInfo.fCX * 0.5f);
	m_tRect.bottom = m_tInfo.fY + (m_tInfo.fCY * 0.5f);
}
