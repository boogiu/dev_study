#include "pch.h"
#include "CGameMgr.h"
#include "Debug.h"

int main()
{
	CGameMgr::GetInstance()->Init();
	while (CGameMgr::GetInstance()->Render()) {}
	CGameMgr::GetInstance()->Release();
	_CrtDumpMemoryLeaks();
}