#include "Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	if (!pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
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
}
