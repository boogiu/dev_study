#include "pch.h"
#include "CColider.h"

CColider::CColider()
	:m_tInfo{}, m_bActive(true), m_tRect{}
{
}

CColider::CColider(INFO _info)
	:m_tInfo(_info), m_bActive(true), m_tRect{}
{
}

CColider::~CColider()
{
}

void CColider::Render(HDC _hDC)
{
	Rectangle(_hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.top);
}

void CColider::Update()
{
	Update_Rect();
}

void CColider::Late_Update()
{
}

void CColider::Update_Rect()
{
	m_tRect.left = m_tInfo.fX-m_tInfo.offsetLeft;
	m_tRect.right = m_tInfo.fX+m_tInfo.offsetRight;

	m_tRect.top = m_tInfo.fY - m_tInfo.offsetTop;
	m_tRect.bottom = m_tInfo.fY + m_tInfo.offsetBottom;
}
