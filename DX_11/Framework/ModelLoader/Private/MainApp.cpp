#include "Loader_Defines.h"
#include "Engine_Defines.h"
#include "Engine_Service.h"

#include "MainApp.h"
#include "GameInstance.h"
#include "IGUIService.h"
#include "ModelLevel.h"

CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Initialize()
{
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);

	ENGINE_DESC desc{};

	desc.hWnd = g_hWnd;
	desc.eWinMode = WINMODE::WIN;
	desc.iWinSizeX = g_iWinSizeX;
	desc.iWinSizeY = g_iWinSizeY;

	if (m_pGameInstance->Init_Engine(desc)) {
		m_pDevice = m_pGameInstance->Get_Device();
		m_pDeviceContext = m_pGameInstance->Get_Context();
	}

	Set_Levels();

	m_pGameInstance->Get_LevelMgr()->Request_ChangeLevel("Model_Level",false);
	
	ImGui::SetCurrentContext(m_pGameInstance->Get_GUISystem()->GetEngineImGuiContext());

	return S_OK;
}

void CMainApp::Update(const float dt)
{
	m_pGameInstance->Update_Engine(dt);
}


HRESULT CMainApp::Render()
{
	_float4 color = { 0.f,0.f,1.f,1.f };
	m_pGameInstance->Draw_Begin(&color);
	m_pGameInstance->Draw();
	m_pGameInstance->Draw_End();
	return S_OK;
}

void CMainApp::Set_Levels() //레벨 등록 함수 ->등록 끝내면
{
	m_pGameInstance->Get_LevelMgr()->Register_Level("Model_Level", []()->CLevel* {return CModelLevel::Create("Model_Level"); });
	m_pGameInstance->Notify_LevelSet(); //레벨 세팅 끝났음을 알림 (게임 인스턴스에게)
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
	m_pGameInstance->Release_Engine();
	m_pGameInstance->DestroyInstance();
}

