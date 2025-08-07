#include "Engine_Define.h"
#include "CCollider.h"
#include "CCollisionMgr.h"

CCollider::CCollider()
	:m_eColType(COLLIDER_LAYER::DEFAULT)
{
}

CCollider::~CCollider()
{
}

void CCollider::Register_System()
{
	CCollisionMgr::GetInstance()->Add_Collider(this);
}
