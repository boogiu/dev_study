#include "pch.h"
#include "CGameMgr.h"

int main()
{
	CGameMgr::GetInstance()->Init();
	while (CGameMgr::GetInstance()->Render()) {}
	CGameMgr::GetInstance()->Release();
}