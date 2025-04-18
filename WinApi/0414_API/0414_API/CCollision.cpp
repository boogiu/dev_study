#include "pch.h"
#include "CCollision.h"

CCollision::CCollision()
{
}

CCollision::~CCollision()
{
}

bool CCollision::CollisionByRect(const RECT& _dst, const RECT& _src)
{
	RECT tmpRect;
	bool bHit = IntersectRect(&tmpRect, &_dst, &_src);
	return bHit;
}

bool CCollision::CollisionByCircle(const INFO& _dst, const INFO& _src)
{
	float fXDistance = (_dst.fX - _src.fX);
	float fYDistance = (_dst.fY - _src.fY);
	float fDistance = sqrtf((fXDistance * fXDistance) + (fYDistance * fYDistance));

	if (fDistance >= ((_dst.fX * 0.5) + (_src.fX * 0.5))) {
		return true;
	}
	else {
		return false;
	}
}

void CCollision::Initialize()
{
}

void CCollision::Update()
{
}

void CCollision::Late_Update()
{
}

void CCollision::Render(HDC _hDC)
{
}

void CCollision::Release()
{
}

void CCollision::UpdateByLayer(LAYER _dst, LAYER _src)
{
}
