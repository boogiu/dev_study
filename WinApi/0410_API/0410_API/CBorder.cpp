#include "pch.h"
#include "CBorder.h"
#include "CPhysicsMgr.h"

CBorder::CBorder()
{
}

CBorder::~CBorder()
{
	Release();
}

void CBorder::Initialize()
{
	m_tInfo.fCX = WINCX;
	m_tInfo.fCY = WINCY;
	m_tInfo.fX = WINCX >> 1;
	m_tInfo.fY = WINCY >> 1;

	CPhysicsMgr::GetInstance().RegisterPhysics(this, GRD_LAYER::BORDER, PHYSICS::COLLIDER);
}

void CBorder::Update()
{
	CTile::Update_Rect();
}

void CBorder::Render(HDC _hDC)
{
	Rectangle(_hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CBorder::Release()
{
}
