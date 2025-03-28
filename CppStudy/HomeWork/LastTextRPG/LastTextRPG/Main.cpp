#include "pch.h"
#include "CGameMgr.h"
int main()
{
	locale::global(std::locale(""));
	wcout.imbue(std::locale());

	CGameMgr::GetInstance().Initialize();
	while (!CGameMgr::GetInstance().isGameEnd())
	{
		CGameMgr::GetInstance().Render();
		CGameMgr::GetInstance().Update();
	}
	CGameMgr::GetInstance().Release();
}
