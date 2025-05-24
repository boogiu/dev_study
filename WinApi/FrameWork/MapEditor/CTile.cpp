#include "pch.h"
#include "../MapEditor/CTile.h"

CTile::CTile() 
	:memDC(nullptr), m_tInfo{}, m_bStick(false)
{
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
}


CTile::~CTile() {
}


void CTile::UI_Set()
{
	m_tInfo.tInfo.fX = 50+ m_tInfo.tSprite.m_Col * 25;
	m_tInfo.tInfo.fY = 50 + m_tInfo.tSprite.m_Row * 25;

	m_tInfo.tInfo.Add_OffsetX(10.f,10.f);
	m_tInfo.tInfo.Add_OffsetY(10.f, 10.f);
}
