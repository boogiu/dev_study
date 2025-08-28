#include "Client_Defines.h"
#include "LoadingLevel.h"
#include "Loader.h"
#include "GameInstance.h"
#include "ILevelService.h"

CLoadingLevel::CLoadingLevel(const string& LevelKey)
	: CLevel{ LevelKey }
{
}

CLoadingLevel::~CLoadingLevel()
{
}

HRESULT CLoadingLevel::Initialize()
{
	string key =CGameInstance::GetInstance()->Get_LevelMgr()->Get_NextLevel();
	m_pLoader = CLoader::Create(key);

	if (!m_pLoader)
		return E_FAIL;

	return S_OK;
}

void CLoadingLevel::Update()
{

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
