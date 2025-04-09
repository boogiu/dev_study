#include "pch.h"
#include "CObj.h"

CObj::CObj()
	: m_fSpeed(0.f), m_bOut(false)
{
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	ZeroMemory(&m_tRect, sizeof(m_tRect));

}

CObj::~CObj()
{
}

void CObj::Update_Rect()
{
	m_tRect.left			=			static_cast<LONG>(m_tInfo.fX	-	(m_tInfo.fCX * 0.5f));
	m_tRect.right			=			static_cast<LONG>(m_tInfo.fX	+	(m_tInfo.fCX * 0.5f));
	m_tRect.top			=			static_cast<LONG>(m_tInfo.fY	-	(m_tInfo.fCY * 0.5f));
	m_tRect.bottom	=			static_cast<LONG>(m_tInfo.fY	+	(m_tInfo.fCY * 0.5f));
}

void CObj::OutOfBorder()
{
	const LONG left = m_tRect.left;
	const LONG right = m_tRect.right;
	const LONG top = m_tRect.top;
	const LONG bottom = m_tRect.bottom;

	if (bottom <= 0 || top >= WINCY || right <= 0 || left >= WINCX)
	{
		m_bOut = true;
	}

	else {
		m_bOut = false;
	}
}
//intersectRect

bool CObj::GetBorderOut()
{
	return m_bOut;
}

INFO CObj::GetPos()
{
	return m_tInfo;
}

void CObj::SetPos(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

const RECT& CObj::GetRECT() const
{
	return m_tRect;
}

