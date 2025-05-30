#include "Engine_Define.h"
#include "CGameObject.h"

CGameObject::CGameObject()
{
}

CGameObject::~CGameObject()
{
}

void CGameObject::Update_Component(float dt)
{
	for (auto& comp : m_Component) {
		comp.get()->Update_Component(dt);
	}
}

void CGameObject::LateUpdate_Component(float dt)
{
	for (auto& comp : m_Component) {
		comp.get()->LateUpdate_Component(dt);
	}
}

void CGameObject::Free()
{
}
