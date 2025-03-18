#include "pch.h"
#include "CGameManager.h"

int main()
{
	CGameManager::GetInstance().Initialize();

	while (CGameManager::GetInstance().GetGameOn())
	{
		CGameManager::GetInstance().Update();
		CGameManager::GetInstance().Render();
	}

	CGameManager::GetInstance().Release();
}
