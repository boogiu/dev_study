#include "pch.h"
#include "CCollision.h"
#include "CObj.h"


bool CCollision::CollisionCheck(CObj* target1, CObj* target2)
{
	RECT tmp{};
	bool bHit = IntersectRect(&tmp, &target1->GetRECT(), &target2->GetRECT());
	return bHit;
}
