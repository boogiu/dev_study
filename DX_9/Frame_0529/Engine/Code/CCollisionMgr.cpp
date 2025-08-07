#include "Engine_Define.h";
#include "CCollisionMgr.h"
#include "CCollider.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
	Free();
}

void CCollisionMgr::Update_Collision(float& dt)
{
	//처리 후에 비우기
	Clear_Container();
}

void CCollisionMgr::LateUpdate_Collision(float& dt)
{
}

void CCollisionMgr::Add_Collider(CCollider* pCol)
{
	COLLIDER_LAYER type = pCol->Get_Layer();
	auto iter = m_ColliderMap[type].find(pCol);

	if (iter != m_ColliderMap[type].end()) return;

	m_ColliderMap[type].insert(pCol);
}

void CCollisionMgr::Remove_Collider(CCollider* pCol)
{
	COLLIDER_LAYER type = pCol->Get_Layer();
	auto iter = m_ColliderMap[type].find(pCol);

	if (iter == m_ColliderMap[type].end()) return;

	m_ColliderMap[type].erase(pCol);
}

void CCollisionMgr::Clear_Container()
{
	for (auto& pair : m_ColliderMap) {
		pair.second.clear();
	}
}

void CCollisionMgr::Free()
{
}
