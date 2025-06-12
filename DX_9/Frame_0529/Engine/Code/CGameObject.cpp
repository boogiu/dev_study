#include "Engine_Define.h"
#include "CGameObject.h"

CGameObject::CGameObject()
{
}

CGameObject::~CGameObject()
{
}

void CGameObject::Update_Component(float& dt)
{
	for (auto& comp : m_DynamicComponent) {
		comp->Update_Component(dt);
	}
}

void CGameObject::LateUpdate_Component(float& dt)
{
	for (auto& comp : m_DynamicComponent) {
		comp->LateUpdate_Component(dt);
	}
}

void CGameObject::Release_Component()
{
	for_each(m_ComponentMap.begin(), m_ComponentMap.end(),
		[](auto& pair) {
			Safe_Release(pair.second);
		});

	m_ComponentMap.clear();
	m_DynamicComponent.clear();
}

void CGameObject::Free()
{
	
}
