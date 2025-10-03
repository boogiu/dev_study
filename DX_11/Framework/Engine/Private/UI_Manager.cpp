#include "UI_Manager.h"
#include "UI_Object.h"
#include "GameInstance.h"
#include "ILevelService.h"

CUI_Manager::CUI_Manager()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

CUI_Manager::~CUI_Manager()
{
}

void CUI_Manager::Post_EngineUpdate(_float dt)
{
	for (auto& pair : m_UIObjects)
		for (auto& UI : pair.second)
			UI->Post_EngineUpdate(dt);
}

void CUI_Manager::Priority_Update(_float dt)
{
	for (auto& pair : m_UIObjects)
		for (auto& UI : pair.second)
			UI->Priority_Update(dt);
}

void CUI_Manager::Update(_float dt)
{
	for (auto& pair : m_UIObjects)
		for (auto& UI : pair.second)
			UI->Update(dt);
}

void CUI_Manager::Late_Update(_float dt)
{
	for (auto& pair : m_UIObjects)
		for (auto& UI : pair.second)
			UI->Late_Update(dt);
}

void CUI_Manager::Clear(const string& LevelTag)
{
	auto iter = m_UIObjects.find(LevelTag);
	if (iter != m_UIObjects.end())
	{
		for (auto& UI : iter->second)
			Safe_Release(UI);
		iter->second.clear();
	}
}

HRESULT CUI_Manager::Sync_To_Level()
{
	ILevelService* pLevelMgr = CGameInstance::GetInstance()->Get_LevelMgr();

	if (!pLevelMgr) {
		MSG_BOX("There is No Level in Level Manager : CPrototypeMgr");
		return E_FAIL;
	}

	vector<string> LevelList = pLevelMgr->Get_LevelList();

	for (string& name : LevelList)
		m_UIObjects.emplace(name, UIobjects());
}

void CUI_Manager::Add_UIObject(CUI_Object* object, const string& level)
{
	if (!m_UIObjects.count(level)) {
		MSG_BOX(" wrong Destination Level  : CUI_Manager");
		return;
	}

	auto& map = m_UIObjects.at(level);
	map.push_back(object);
}

CUI_Manager* CUI_Manager::Create()
{
	CUI_Manager* pInstance = new CUI_Manager();
	return pInstance;
}

void CUI_Manager::Free()
{
	__super::Free();

	for (auto& pair : m_UIObjects)
		for (auto& UI : pair.second)
			Safe_Release(UI);

	Safe_Release(m_pGameInstance);
}


