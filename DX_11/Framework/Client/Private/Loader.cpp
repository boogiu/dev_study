#include "Client_Defines.h"
#include "Loader.h"
#include "LogoLevel.h"
#include "GamePlayLevel.h"

CLoader::CLoader()
{
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
    CoInitializeEx(nullptr, 0);

    EnterCriticalSection(&m_CriticalSection);
    /*로딩 로직*/
    if (m_sNextLevel == "Logo_Level")
        CLogoLevel::PreLoad_Level();    

    if (m_sNextLevel == "GamePlay_Level")
        CGamePlayLevel::PreLoad_Level();

    LeaveCriticalSection(&m_CriticalSection);

    return S_OK;
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
}
