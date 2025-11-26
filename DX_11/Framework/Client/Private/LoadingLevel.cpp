#include "Client_Defines.h"
#include "LoadingLevel.h"
#include "Loader.h"
#include "GameInstance.h"
#include "ILevelService.h"
#include "IProtoService.h"
#include "IObjectService.h"
#include "IResourceService.h"
#include "IUI_Service.h"
#include "Builder.h"
#include "UI_Object.h"

CLoadingLevel::CLoadingLevel(const string& LevelKey)
	:CLevel{ LevelKey }
{

}

CLoadingLevel::~CLoadingLevel()
{
}

HRESULT CLoadingLevel::Initialize()
{
	string LevelID = CGameInstance::GetInstance()->Get_LevelMgr()->Get_NextLevel();
	m_pLoader = CLoader::Create(LevelID);

	if (!m_pLoader)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingLevel::Awake()
{
	CUI_Object* pObj = Builder::Create_UIObject({ G_GlobalLevelKey, "GamePlay_UI_Loading"})
		.Add_To_Level("Loading_Level")
		.Position({ Client::g_iWinSizeX * 0.5f, Client::g_iWinSizeY * 0.5f })
		.Scale({ Client::g_iWinSizeX,Client::g_iWinSizeY })
		.Build("Background");

	CGameInstance::GetInstance()->Get_UIMgr()->Add_UIObject(pObj, "Loading_Level");

	return S_OK;
}

void CLoadingLevel::Update()
{
	auto Timer = CGameInstance::GetInstance()->Get_TimeMgr();
	m_fLoadingTime += Timer->Get_DeltaTime("Timer_Frame60");

	if (m_pLoader->isFinished()) {
		CGameInstance::GetInstance()->Get_LevelMgr()->Notify_LoadComplete();
	}
}

HRESULT CLoadingLevel::Render()
{
	SetWindowText(g_hWnd, TEXT("로딩레벨입니다."));
	return S_OK;
}

CLoadingLevel* CLoadingLevel::Create(const string& LevelKey)
{
	CLoadingLevel* instance = new CLoadingLevel(LevelKey);

	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

void CLoadingLevel::Free()
{
	__super::Free();
	Safe_Release(m_pLoader);
}
