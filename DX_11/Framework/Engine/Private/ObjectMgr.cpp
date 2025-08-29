#include "ObjectMgr.h"
#include "GameInstance.h"
#include "ILevelService.h"
#include "IProtoService.h"
#include "GameObject.h"
#include "Layer.h"
#include "Builder.h"
#include "ILevelService.h"

CObjectMgr::CObjectMgr()
	: m_pGameInstance(CGameInstance::GetInstance())
{
}

HRESULT CObjectMgr::Initialize()
{
	return S_OK;
}

void CObjectMgr::Priority_Update(_float dt)
{
	for (auto& pair : m_Layers)
		for (auto& layers : pair.second)
			layers.second->Priority_Update(dt);
}

void CObjectMgr::Update(_float dt)
{
	for (auto& pair : m_Layers)
		for (auto& layers : pair.second)
			layers.second->Update(dt);
}

void CObjectMgr::Late_Update(_float dt)
{
	for (auto& pair : m_Layers)
		for (auto& layers : pair.second)
			layers.second->Late_Update(dt);
}

CBuilder CObjectMgr::Create_Object(const CLONE_DESC clone)
{
	_bool result = false;
	return CBuilder(clone,&result);
}

void CObjectMgr::Add_Object(CGameObject* object, const LAYER_DESC* layer)
{
	if (!m_Layers.count(layer->DestLevel)) {
		MSG_BOX(" wrong Destination Level  : CObjectMgr");
		return;
	}

	auto& map = m_Layers.at(layer->DestLevel);
	auto iter = map.find(layer->LayerTag);

	if (iter == map.end()) {
		CLayer* newLayer = CLayer::Create();
		newLayer->Add_GameObject(object);
		map.emplace(layer->LayerTag, newLayer);
	}
	else {
		iter->second->Add_GameObject(object);
	}
}

void CObjectMgr::Clear(const string& LevelTag)
{

	if (!m_Layers.count(LevelTag)) {
		MSG_BOX("There is No Same Level Tag  : CObjectMgr");
		return;
	}

	for (auto& pair : m_Layers[LevelTag]) {
		Safe_Release(pair.second);
	}
}

HRESULT CObjectMgr::Sync_To_Level()
{
	ILevelService* pLevelMgr = CGameInstance::GetInstance()->Get_LevelMgr();

	if (!pLevelMgr) {
		MSG_BOX("There is No Level in Level Manager : CPrototypeMgr");
		return E_FAIL;
	}

	vector<string> LevelList = pLevelMgr->Get_LevelList();

	for (string& name : LevelList)
		m_Layers.emplace(name, LAYERS{});
}

const unordered_map<string, class CLayer*>& CObjectMgr::Get_LevelLayer(const string& LevelTag)
{
	auto iter = m_Layers.find(LevelTag);
	if (iter == m_Layers.end())
		return unordered_map<string, class CLayer*>();
	else
		return iter->second;
}

CObjectMgr* CObjectMgr::Create()
{
	CObjectMgr* pInstance = new CObjectMgr();

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObjectMgr::Free()
{
	__super::Free();

	for (auto& pair : m_Layers) {
		for (auto& pair2 : pair.second)
			Safe_Release(pair2.second);
		pair.second.clear();
	}
	m_Layers.clear();
}
