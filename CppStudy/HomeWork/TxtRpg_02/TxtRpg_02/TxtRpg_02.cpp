#include "pch.h"
#include "CGameManager.h"

int main()
{
    CGameManager gameManger;
    gameManger.Init();
    while (gameManger.Render()){}
    gameManger.Release();
}
