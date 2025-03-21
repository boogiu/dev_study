#include "pch.h"
#include "CGameManager.h"
#include "Debug.h"

int main()
{
    CGameManager gameManger;
    gameManger.Init();
    while (gameManger.Render()){}
    gameManger.Release();
    _CrtDumpMemoryLeaks();
}
