#include "pch.h"
#include "CPhysicsMgr.h"
#include "Physics.h"
#include "CObject.h"

CPhysicsMgr::CPhysicsMgr()
	:m_Velocity(nullptr),
	m_Gravity(nullptr), 
	m_Collision(nullptr), 
	m_vecColliderObj(static_cast<int>(OBJ_LAYER::END)), 
	m_vecColliderGrd(static_cast<int>(GRD_LAYER::END))
{

}

CPhysicsMgr::~CPhysicsMgr()
{
	Release();
}

void CPhysicsMgr::Initialize()
{
	
	m_Velocity = new CVelocity;
	m_Velocity->Initialize();

	m_Gravity = new CGravity;
	m_Gravity->Initialize();

	m_Collision = new CCollision;
	m_Collision->Initialize();
}

void CPhysicsMgr::Update()
{
	m_Velocity->Update();
	m_Gravity->Update();
	m_Collision->Update();
}

void CPhysicsMgr::Render()
{

}

void CPhysicsMgr::Release()
{
	Safe_Delete<CVelocity*>(m_Velocity);
	Safe_Delete<CGravity*>(m_Gravity);
	Safe_Delete<CCollision*>(m_Collision);
}

void CPhysicsMgr::RegisterPhysics(CObject* _obj, OBJ_LAYER _layer, PHYSICS _physics)
{
	int iLayer = static_cast<int>(_layer);
	if (iLayer < 0 || iLayer >= m_vecColliderObj.size())
		return;

	switch (_physics)
	{
	case PHYSICS::COLLIDER:
		TryAdd(m_vecColliderObj[iLayer], _obj);
		break;

	case PHYSICS::GRAVITY:
		TryAdd(m_vecGravityObj, _obj);
		break;

	case PHYSICS::VELOCITY:
		TryAdd(m_vecVelocityObj, _obj);
		break;

	case PHYSICS::ALL:
		RegisterPhysics(_obj, _layer, PHYSICS::COLLIDER);
		RegisterPhysics(_obj, _layer, PHYSICS::GRAVITY);
		RegisterPhysics(_obj, _layer, PHYSICS::VELOCITY);
		break;

	case PHYSICS::END:
	default:
		break;
	}
}


void CPhysicsMgr::RegisterPhysics(CTile* _tile, GRD_LAYER _layer, PHYSICS _physics)
{
	int iLayer = static_cast<int>(_layer);
	if (iLayer < 0 || iLayer >= m_vecColliderGrd.size())
		return;

	switch (_physics)
	{
	case PHYSICS::COLLIDER:
		TryAdd(m_vecColliderGrd[iLayer], _tile);
		break;
	case PHYSICS::GRAVITY:
		TryAdd(m_vecGravityGrd, _tile);
		break;

	case PHYSICS::VELOCITY:
		TryAdd(m_vecVelocityGrd, _tile);
		break;

	case PHYSICS::ALL:
		RegisterPhysics(_tile, _layer, PHYSICS::COLLIDER);
		RegisterPhysics(_tile, _layer, PHYSICS::GRAVITY);
		RegisterPhysics(_tile, _layer, PHYSICS::VELOCITY);
		break;

	case PHYSICS::END:
	default:
		break;
	}
}

vector<CObject*>* CPhysicsMgr::GetPhysicsObject(PHYSICS _physics)
{
	if (_physics == PHYSICS::GRAVITY) {
		return &m_vecGravityObj;
	}
	else if (_physics == PHYSICS::VELOCITY) {
		return &m_vecVelocityObj;
	}
	return nullptr;
}


vector<CTile*>* CPhysicsMgr::GetPhysicsTile(PHYSICS _physics)
{
	if (_physics == PHYSICS::GRAVITY) {
		return &m_vecGravityGrd;
	}
	else if (_physics == PHYSICS::VELOCITY) {
		return &m_vecVelocityGrd;
	}
	return nullptr;
}

vector<vector<CObject*>>* CPhysicsMgr::GetCollisionObject()
{
	return &m_vecColliderObj;
}

vector<vector<CTile*>>* CPhysicsMgr::GetCollisionGround()
{
	return &m_vecColliderGrd;
}

void CPhysicsMgr::TryAdd(vector<CObject*>& container, CObject* obj)
{
	if (find(container.begin(), container.end(), obj) == container.end())
		container.push_back(obj);
}

void CPhysicsMgr::TryAdd(vector<CTile*>& container, CTile* _tile)
{
	if (find(container.begin(), container.end(), _tile) == container.end())
		container.push_back(_tile);
}
