#include "pch.h"
#include "CBullet.h"



CBullet::CBullet(short sDir)
{
	m_Dir = sDir;
}

CBullet::~CBullet()
{
	Release();
}

void CBullet::Release()
{
}

void CBullet::Initialize()
{

	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;

	m_fSpeed = 15.f;
}

void CBullet::Update()
{
	CObj::Update_Rect();
	CObj::OutOfBorder();

	MoveBullet();
}

void CBullet::Render(HDC hdc)
{
	Ellipse(hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}



void CBullet::MoveBullet()
{

	if (m_Dir & sUpDir) {
		m_tInfo.fY -= m_fSpeed;
	}
	if (m_Dir & sDownDir) {
		m_tInfo.fY += m_fSpeed;
	}
	if (m_Dir & sLeftDir) {
		m_tInfo.fX -= m_fSpeed;
	}
	if (m_Dir & sRightDir) {
		m_tInfo.fX += m_fSpeed;
	}
}
