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

	_uint ObjectIndex = m_GameObjects.size();
	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		/*벡터를 순회하면서 널포인터가 있는지 검색*/
		if (m_GameObjects[i] == nullptr) {
			ObjectIndex = i;
			break;
		}
	}

	if (m_IndexByID.count(pGameObject->Get_ObjectID()))
		return E_FAIL;

	/*같은 ID의 오브젝트가 없다면*/
	if(ObjectIndex == m_GameObjects.size()) /*마지막에 추가*/
		m_GameObjects.push_back(pGameObject);
	else
		m_GameObjects[ObjectIndex] = pGameObject;

	pGameObject->Set_Layer(this);
	m_IndexByID[pGameObject->Get_ObjectID()] = ObjectIndex;
	return S_OK;
}

void CLayer::Pre_EngineUpdate(_float dt)
{
	for (auto& pGameObject : m_GameObjects)
		if (pGameObject && pGameObject->Is_Root())
			pGameObject->Pre_EngineUpdate(dt);
}

void CLayer::Post_EngineUpdate(_float dt)
{
	for (auto& pGameObject : m_GameObjects)
		if (pGameObject && pGameObject->Is_Root())
			pGameObject->Post_EngineUpdate(dt);
}

void CLayer::Priority_Update(_float dt)
{
	for (auto& pGameObject : m_GameObjects)
		if (pGameObject && pGameObject->Is_Root())
			pGameObject->Priority_Update(dt);
}

void CLayer::Update(_float dt)
{
	for (auto& pGameObject : m_GameObjects)
		if (pGameObject && pGameObject->Is_Root())
			pGameObject->Update(dt);
}

void CLayer::Late_Update(_float dt)
{
	for (auto& pGameObject : m_GameObjects)
		if (pGameObject && pGameObject->Is_Root())
			pGameObject->Late_Update(dt);
}

CGameObject* CLayer::Pop_GameObject(_uint ObjectID)
{
	CGameObject* Poped = { nullptr };

	auto iter = m_IndexByID.find(ObjectID);

	if (iter == m_IndexByID.end())
		return nullptr;

	else {
		_uint ObjectIndex = iter->second;
		Poped =  m_GameObjects[ObjectIndex];
		m_GameObjects[ObjectIndex] = nullptr;
		m_IndexByID.erase(iter);
		Poped->Set_Layer(nullptr);
	}

	return Poped;
}

void CLayer::Remove_GameObject(_uint ObjectID)
{
	auto iter = m_IndexByID.find(ObjectID);

	if (iter == m_IndexByID.end())
		return ;

	else {
		_uint ObjectIndex = iter->second;
		m_GameObjects[ObjectIndex]->Set_Layer(nullptr);
		Safe_Release(m_GameObjects[ObjectIndex]);
		m_GameObjects[ObjectIndex] = nullptr;
		m_IndexByID.erase(iter);
	}
}

CGameObject* CLayer::Find_ObjectByID(_uint ObjectID)
{
	auto iter = m_IndexByID.find(ObjectID);
	if (iter == m_IndexByID.end())
		return nullptr;

	else {
		_uint ObjectIndex = iter->second;
		return m_GameObjects[ObjectIndex];
	}
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
	m_IndexByID.clear();
}
