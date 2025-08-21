#include "LevelMgr.h"
#include "Level.h"

CLevelMgr::CLevelMgr()
{
}

CLevelMgr::~CLevelMgr()
{
}

HRESULT CLevelMgr::Request_ChangeLevel(string key)
{
    if (!m_LevelCreators.count(key))
        return E_FAIL;

    m_NextLevelTag = key;
    m_eState = LEVEL_STATE::REQUEST;

    return S_OK;
}

void CLevelMgr::Update(_float dt)
{
    if (nullptr == m_pCurrentLevel)
        return;

    switch (m_eState)
    {
    case Engine::LEVEL_STATE::INITIAL:
        break;
    case Engine::LEVEL_STATE::REQUEST:
        if (!m_LoadingLevelKey.empty() && m_LevelCreators.count(m_LoadingLevelKey)) {
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

    m_pCurrentLevel->Update(dt);
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
       return;
   }

   m_LevelCreators.insert({ key,creator });
}

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
