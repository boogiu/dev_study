#include "pch.h"
#include "CCollision.h"
#include "CObject.h"
#include "CPhysicsMgr.h"
#include "CTile.h"

bool CCollision::OnCollision(RECT target1, RECT target2)
{
	RECT tmpRect;
	bool b_onHit = IntersectRect(&tmpRect, &target1, &target2);
	return b_onHit;
}

CCollision::CCollision()
	:m_pCollisionObj(nullptr),m_pCollisionGrd(nullptr)
{
}

CCollision::~CCollision()
{
	Release();
}

void CCollision::Initialize()
{
}

void CCollision::Update()
{
	if (!m_pCollisionObj) {
		m_pCollisionObj = CPhysicsMgr::GetInstance().GetCollisionObject();
	}
	if (!m_pCollisionGrd) {
		m_pCollisionGrd = CPhysicsMgr::GetInstance().GetCollisionGround();
	}

	Collision_Object(OBJ_LAYER::PLAYER, OBJ_LAYER::ENEMY);
	Collision_Object(OBJ_LAYER::ENEMY, OBJ_LAYER::BULLET);

	Collision_Ground(OBJ_LAYER::BULLET, GRD_LAYER::BORDER);
}

void CCollision::Render()
{
}

void CCollision::Release()
{
}

void CCollision::Collision_Object(OBJ_LAYER _Main, OBJ_LAYER _Sub)
{
	int iMain = static_cast<int>(_Main);
	int iSub = static_cast<int>(_Sub);

	auto MainIter = (*m_pCollisionObj)[iMain].begin();
	for (;MainIter != (*m_pCollisionObj)[iMain].end(); ++MainIter) {
	
		if (!(*MainIter)->GetActivate()) { continue; } 

		auto SubIter = (*m_pCollisionObj)[iSub].begin();

		for (;SubIter != (*m_pCollisionObj)[iSub].end(); ++SubIter) {
			
			if ((*SubIter) == (*MainIter)) { continue; }
			if (!(*SubIter)->GetActivate()) { continue; }

			RECT& playerRect = (*MainIter)->GetRect();
			RECT& enemyRect = (*SubIter)->GetRect();

			if (OnCollision(playerRect, enemyRect)) {
				(*MainIter)->OnCollisionEnter(*SubIter);
				(*SubIter)->OnCollisionEnter(*MainIter);
			}
			else {
			
			}
		}
	}
}


void CCollision::Collision_Ground(OBJ_LAYER _obj, GRD_LAYER _grd)
{
	int iObj = static_cast<int>(_obj);
	int iGrd = static_cast<int>(_grd);

	auto ObjIter = (*m_pCollisionObj)[iObj].begin();

	for (;ObjIter != (*m_pCollisionObj)[iObj].end(); ++ObjIter) {

		if (!(*ObjIter)->GetActivate()) { continue; }

		auto GrdIter = (*m_pCollisionGrd)[iGrd].begin();

		for (;GrdIter != (*m_pCollisionGrd)[iGrd].end(); ++GrdIter) {

			RECT& objectRect = (*ObjIter)->GetRect();
			RECT& groundRect = (*GrdIter)->GetRect();

			if (OnCollision(objectRect, groundRect)) {
				(*ObjIter)->OnCollisionEnter(*GrdIter);
			}
			else {
				(*ObjIter)->OnCollisionOut(*GrdIter);
			}
		}
	}
}