#include "Level.h"
#include "GameInstance.h"
#include "ILevelService.h"
CLevel::CLevel(const string& LevelKey)
	:m_LevelKey{ LevelKey }
{
}

HRESULT CLevel::Render(ID3D11DeviceContext* pContext)
{
	return S_OK;
}

void CLevel::Free()
{
	//CGameInstance::GetInstance()->Get_LevelMgr().clear
}
