#include "Client_Defines.h"
#include "LogoLevel.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "ILevelService.h"

CLogoLevel::CLogoLevel(const string& LevelKey)
	: CLevel{ LevelKey },
	m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CLogoLevel::Initialize()
{
	return S_OK;
}

void CLogoLevel::Update()
{
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_SPACE)) {
		CGameInstance::GetInstance()->Get_LevelMgr()->Request_ChangeLevel("GamePlay_Level");
	}
}

HRESULT CLogoLevel::Render()
{
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
	return S_OK;
}

CLogoLevel* CLogoLevel::Create(const string& LevelKey)
{
	CLogoLevel* instance = new CLogoLevel(LevelKey);
	if (FAILED(instance->Initialize())) {
		MSG_BOX("LOGO level Create Failed");
		Safe_Release(instance);
	}

	return instance;
}

void CLogoLevel::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
}

void CLogoLevel::PreLoad_Level()
{

}
