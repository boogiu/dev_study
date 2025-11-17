#include "Client_Defines.h"
#include "EventSystem.h"

CEventSystem::CEventSystem()
{
}

CEventSystem::CEventSystem(const CEventSystem& rhs)
	:CLevelObject(rhs)
{
}

CEventSystem::~CEventSystem()
{
}

HRESULT CEventSystem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEventSystem::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CEventSystem::Priority_Update(_float dt)
{
}

void CEventSystem::Update(_float dt)
{
}

void CEventSystem::Late_Update(_float dt)
{
}

void CEventSystem::UnregisterAll(CGameObject* pObj)
{
	for (auto& [typeIndex, structPtr] : m_Callbacks)
	{
		structPtr->RemoveByOwner(pObj);
	}
}

CEventSystem* CEventSystem::Create()
{
	return new CEventSystem;
}

CGameObject* CEventSystem::Clone(INIT_DESC* pArg)
{
	return new CEventSystem(*this);
}

void CEventSystem::Free()
{
	__super::Free(); 

	for (auto& [typeID, structPointer] : m_Callbacks) {
		Safe_Delete(structPointer);
	}

	m_Callbacks.clear();
}
