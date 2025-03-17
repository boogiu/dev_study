#include "pch.h"
#include "CGameManager.h"

int main()
{
	CGameManager::GetInstance().Initialize();

	while (true)
	{
		CGameManager::GetInstance().Update();
		CGameManager::GetInstance().Render();
	}

	CGameManager::GetInstance().Release();
}
