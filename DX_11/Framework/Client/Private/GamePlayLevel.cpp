#include "Client_Defines.h"
#include "GamePlayLevel.h"
#include "GameInstance.h"
#include "ILevelService.h"
CGamePlayLevel::CGamePlayLevel(const string& LevelKey)
    :CLevel{ LevelKey },
    m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CGamePlayLevel::Initialize()
{
    return S_OK;
}

void CGamePlayLevel::Update()
{
    m_pGameInstance->Get_LevelMgr()->Request_ChangeLevel("¾ÈÁ¦Çö");
}

HRESULT CGamePlayLevel::Render()
{
    return S_OK;
}

CGamePlayLevel* CGamePlayLevel::Create(const string& LevelKey)
{
    CGamePlayLevel* instance = new CGamePlayLevel(LevelKey);
    if (FAILED(instance->Initialize())) {
        MSG_BOX("GamePlay level Create Failed");
        Safe_Release(instance);
    }
    return instance;
}

void CGamePlayLevel::Free()
{
    Safe_Release(m_pGameInstance);
}
