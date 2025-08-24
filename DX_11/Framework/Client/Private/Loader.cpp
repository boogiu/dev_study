#include "Client_Defines.h"
#include "Loader.h"
#include "GameInstance.h"
#include "IProtoService.h"
#include "BackGround.h"

CLoader::CLoader()
    : m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
}

CLoader::~CLoader()
{
}

HRESULT CLoader::Initialize(const string& nextLV)
{
    InitializeCriticalSection(&m_CriticalSection);

    m_hThread = (HANDLE)_beginthreadex(nullptr, 0, CLoader::LoadingThread, this, 0, nullptr);
    m_sNextLevel = nextLV;

    return S_OK;
}


_uint WINAPI  CLoader::LoadingThread(void* pArg)
{
    CLoader* pLoader = static_cast<CLoader*>(pArg);

    if (FAILED(pLoader->Loading()))
        return 1;

    pLoader->Set_Finished();
    return 0;
}

HRESULT CLoader::Loading()
{
    EnterCriticalSection(&m_CriticalSection);
    /*로딩 로직*/
    if (m_sNextLevel == "Logo_Level")
        Load_LogoLevel();

    LeaveCriticalSection(&m_CriticalSection);

    return S_OK;
}

void CLoader::Load_LogoLevel()
{
    IProtoService* pProtoMgr = m_pGameInstance->Get_PrototypeMgr();

   pProtoMgr->Add_ProtoType("Logo_Level","Proto_GameObject_Background", CBackGround::Create());
}

CLoader* CLoader::Create(const string& nextLV)
{
    CLoader* instance = new CLoader();

    if (FAILED(instance->Initialize(nextLV))) {
        Safe_Release(instance);
        instance = nullptr;
    }

    return instance;
}

void CLoader::Free()
{
    __super::Free();	
    WaitForSingleObject(m_hThread, INFINITE);
    CloseHandle(m_hThread);
    DeleteCriticalSection(&m_CriticalSection);
    m_pGameInstance->DestroyInstance();

}
