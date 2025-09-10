#include "Layer.h"
#include "GameObject.h"
#include "GUIWidget.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	if (!pGameObject)
		return E_FAIL;

	auto result = m_GameObjectsByID.emplace(pGameObject->Get_ObjectID(), pGameObject);

	if (result.second) {
		m_GameObjects.push_back(pGameObject);
		Safe_AddRef(pGameObject);
		return S_OK;
	}
	else
		return E_FAIL;
}

void CLayer::Engine_Update(_float dt)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Engine_Update(dt);
}

void CLayer::Priority_Update(_float dt)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Priority_Update(dt);
}

void CLayer::Update(_float dt)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Update(dt);
}

void CLayer::Late_Update(_float dt)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Late_Update(dt);
}

CGameObject* CLayer::Find_ObjectByID(_uint ID)
{
	auto iter = m_GameObjectsByID.find(ID);
	if (iter == m_GameObjectsByID.end())
		return nullptr;

	else
		return iter->second;
}

CLayer* CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();


	for (auto& Pair : m_GameObjectsByID)
		Safe_Release(Pair.second);

	m_GameObjectsByID.clear();
}
