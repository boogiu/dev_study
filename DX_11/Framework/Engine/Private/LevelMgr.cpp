#include "LevelMgr.h"
#include "Level.h"

CLevelMgr::CLevelMgr()
{
}

CLevelMgr::~CLevelMgr()
{
}

HRESULT CLevelMgr::Request_ChangeLevel(string key,_bool Load)
{
    if (!m_LevelCreators.count(key))
        return E_FAIL;

    m_NextLevelTag = key;
    if (Load)
        m_eState = LEVEL_STATE::REQUEST;
    else
        m_eState = LEVEL_STATE::LOADED;

    return S_OK;
}

void CLevelMgr::Update(_float dt)
{
    switch (m_eState)
    {
    case Engine::LEVEL_STATE::INITIAL:
        break;
    case Engine::LEVEL_STATE::REQUEST:
        if (!m_LoadingLevelKey.empty() && m_LevelCreators.count(m_LoadingLevelKey)) {
            //로딩 레벨이 있다면.
            Safe_Release(m_pCurrentLevel);
            m_pCurrentLevel = m_LevelCreators[m_LoadingLevelKey]();
            m_eState = LEVEL_STATE::LOADING;
        }
        else {
            m_eState = LEVEL_STATE::LOADED;
        }
        break;

    case Engine::LEVEL_STATE::LOADED:
        if (!m_NextLevelTag.empty() && m_LevelCreators.count(m_NextLevelTag)) {
            Safe_Release(m_pCurrentLevel);
            m_pCurrentLevel = m_LevelCreators[m_NextLevelTag]();
            m_eState = LEVEL_STATE::STABLE;
            m_NextLevelTag.clear();
        }
        break;
    case Engine::LEVEL_STATE::STABLE:
        break;
    default:
        break;
    }

    m_pCurrentLevel->Update();
}

HRESULT CLevelMgr::Render()
{
    if (nullptr == m_pCurrentLevel)
        return E_FAIL;

    return m_pCurrentLevel->Render();
}

void CLevelMgr::Register_Level(string key, LEVEL_CREATOR creator)
{
   auto iter =  m_LevelCreators.find(key);

   if (iter != m_LevelCreators.end()) {
       MSG_BOX("Level Already Exist : CLevelMgr");
       return;
   }

   m_LevelCreators.insert({ key,creator });
}

#pragma region For_OtherManager

const vector<string> CLevelMgr::Get_LevelList()
{
    vector<string> nameList;

    for (auto& pair : m_LevelCreators) {
        nameList.push_back(pair.first);
    }

    return nameList;
}

_bool CLevelMgr::Check_ValidateLevel(const string& LevelTag)
{
    return m_LevelCreators.count(LevelTag);
}

#pragma endregion

#pragma region For_LoadingLevel

void CLevelMgr::Set_LoadingLevel(const string& LoadingKey)
{
    m_LoadingLevelKey = LoadingKey;
}

void CLevelMgr::Notify_LoadComplete()
{
    m_eState = LEVEL_STATE::LOADED;
}

#pragma endregion

CLevelMgr* CLevelMgr::Create()
{
    return new CLevelMgr();
}

void CLevelMgr::Free()
{
    __super::Free();

    Safe_Release(m_pCurrentLevel);
}
