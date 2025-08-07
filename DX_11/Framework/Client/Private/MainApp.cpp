#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "Engine_Service.h"

#include "MainApp.h"
#include "GameInstance.h"

#include "LoadingLevel.h"

CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Initialize()
{
	m_pGameInstance = CGameInstance::GetInstance();
	//Safe_AddRef(m_pGameInstance); ==> 수업 때에는 유지. 이후에 해결 예정

	ENGINE_DESC desc{};

	desc.hWnd = g_hWnd;
	desc.eWinMode = WINMODE::WIN;
	desc.iWinSizeX = g_iWinSizeX;
	desc.iWinSizeY = g_iWinSizeY;

	if (m_pGameInstance->InitSystems(desc)) {
		m_pDevice = m_pGameInstance->Get_Device();
		m_pDeviceContext = m_pGameInstance->Get_Context();
	}
	
	Set_Levels();

	return S_OK;

}

void CMainApp::Update(const float dt)
{
	m_pGameInstance->Update_Engine(dt);
}

void CMainApp::Late_Update(const float dt)
{
}

HRESULT CMainApp::Render()
{
	_float4 color = { 0.f,0.f,1.f,1.f };
	m_pGameInstance->Draw_Begin(&color);
	m_pGameInstance->Draw();
	m_pGameInstance->Draw_End();
	return S_OK;
}

void CMainApp::Set_Levels()
{
	m_pGameInstance->Get_Service<ILevelService>()->Register_Level("Loading_Level", []()->CLevel* {return CLoadingLevel::Create(); });
	m_pGameInstance->Get_Service<ILevelService>()->Set_LoadingLevel("Loading_Level");
	/*이후로 계속*/

}

CMainApp* CMainApp::Create()
{
	CMainApp* instance = new CMainApp();

	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

void CMainApp::Free()
{
	__super::Free();
	m_pGameInstance->DestroyInstance();
}

