#include "pch.h"
#include "CGameManager.h"
#include "Debug.h"

int main()
{
    CGameManager::GetInstance().Init();
    while (CGameManager::GetInstance().Render()){}
    CGameManager::GetInstance().Release();

    _CrtDumpMemoryLeaks();
}
