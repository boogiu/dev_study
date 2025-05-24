#include "pch.h"
#include "CCollider.h"
#include "CCamera.h"

CCollider::CCollider(CObject* owner)
	:m_tInfo{}, m_bActive(true), m_tRect{}, m_pOwner(owner)
{
}

CCollider::CCollider(CObject* owner, INFO _info)
	:m_tInfo{}, m_bActive(true), m_tRect{}, m_pOwner(owner)
{
	m_tInfo.Translate_Info(_info);
}

CCollider::CCollider(CObject* owner, COLLIDER_INFO _info)
	:m_tInfo{}, m_bActive(true), m_tRect{}, m_pOwner(owner)
{
	m_tInfo.Copy(_info);
}

CCollider::~CCollider()
{
}

void CCollider::Render(HDC _hDC)
{
	INFO_EX worldInfo = CCamera::Get_Instance()->WorldToScreen(m_tInfo);

	// 초록색 펜 (테두리용)
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	// 내부를 비우기 위해 NULL_BRUSH 사용
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, GetStockObject(NULL_BRUSH));

	Rectangle(_hDC,
		(int)(worldInfo.fX - worldInfo.offsetLeft),
		(int)(worldInfo.fY - worldInfo.offsetTop),
		(int)(worldInfo.fX + worldInfo.offsetRight),
		(int)(worldInfo.fY + worldInfo.offsetBottom)
	);

	// 원래 브러시와 펜으로 복원 및 자원 해제
	SelectObject(_hDC, hOldBrush);
	SelectObject(_hDC, hOldPen);
	DeleteObject(hPen);
}


void CCollider::Update()
{
	m_tRect.left = m_tInfo.fX - m_tInfo.offsetLeft;
	m_tRect.right = m_tInfo.fX + m_tInfo.offsetRight;
	m_tRect.top = m_tInfo.fY - m_tInfo.offsetTop;
	m_tRect.bottom = m_tInfo.fY + m_tInfo.offsetBottom;
}

void CCollider::Late_Update()
{
}

void CCollider::Update_Rect()
{

}
