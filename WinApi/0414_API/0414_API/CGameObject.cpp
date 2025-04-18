#include "pch.h"
#include "CGameObject.h"
#include "CComponent.h"
CGameObject::CGameObject()
{
}

CGameObject::~CGameObject()
{
	if (m_vecComponent.empty()) return;
	for (CComponent* component : m_vecComponent) {
		if(component)
			Safe_Delete<	CComponent*>(component);
	}
}

void CGameObject::AddComponent(CComponent* component)
{
	component->SetOwner(this);
	m_vecComponent.push_back(component);
}

void CGameObject::Update_Component()
{
	if (m_vecComponent.empty()) return;

	for (CComponent* component : m_vecComponent) {
		if (component)
			component->Update();
	}
}

void CGameObject::Render_Component(HDC _hDC)
{
	if (m_vecComponent.empty()) return;

	for (CComponent* component : m_vecComponent) {
		if (component)
			component->Render(_hDC);
	}
}
