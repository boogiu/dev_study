#include "Level.h"
#include "GameInstance.h"
#include "ILevelService.h"
CLevel::CLevel(const string& LevelKey)
	:m_LevelKey{ LevelKey }
{
}

void CLevel::Free()
{
	//CGameInstance::GetInstance()->Get_LevelMgr().clear
}
