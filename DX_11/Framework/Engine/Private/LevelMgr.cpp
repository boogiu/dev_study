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
    if (Load) //로딩 설정이 없으면 그냥 바로 로드되었다 치고 다음레벨로 넘어감.
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
    case Engine::LEVEL_STATE::REQUEST: //다음 레벨로 가는 것을 요청한 상태
        if (!m_LoadingLevelKey.empty() && m_LevelCreators.count(m_LoadingLevelKey)) {
            //로딩 레벨이 있다면.
            Safe_Release(m_pCurrentLevel); //자원 해제(이전 레벨)
            m_pCurrentLevel = m_LevelCreators[m_LoadingLevelKey]();
            m_eState = LEVEL_STATE::LOADING; //로딩 레벨 설정이 되어 있으면 로딩을 진행함.
        }
        else {
            m_eState = LEVEL_STATE::LOADED;
        }
        break;

        //이 사이는 강제적으로 로딩레벨을 거치고 가게 함.

    case Engine::LEVEL_STATE::LOADED: //로딩이 완료되면 아까 요청한 레벨로 전환
        if (!m_NextLevelTag.empty() && m_LevelCreators.count(m_NextLevelTag)) {
            Safe_Release(m_pCurrentLevel);//자원 해제 (로딩 레벨)
            m_pCurrentLevel = m_LevelCreators[m_NextLevelTag](); 
            m_eState = LEVEL_STATE::STABLE;
            m_NextLevelTag.clear();
        }
        break;
    case Engine::LEVEL_STATE::STABLE: //안정적으로 레벨이 돌아가고 있는 상태 -> 레벨 체인지 요청이 안들어온 상태
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
    m_LoadingLevelKey = LoadingKey; //셋로딩 안하면 로딩레벨 없음. (레벨 키를 지금 스트링으로 햇어)
    //나중에 JSON으로 맵에디터나 이런거 레벨 이름으로 받아올 것 같아서
}

void CLevelMgr::Notify_LoadComplete()
{
    m_eState = LEVEL_STATE::LOADED; //로디드로 변함
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
