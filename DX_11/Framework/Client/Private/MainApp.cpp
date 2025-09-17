#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "Engine_Service.h"

#include "MainApp.h"
#include "GameInstance.h"

#include "LoadingLevel.h"
#include "LogoLevel.h"

//		D3D11_FILTER Filter;
//		D3D11_TEXTURE_ADDRESS_MODE AddressU;
//		D3D11_TEXTURE_ADDRESS_MODE AddressV;
//		D3D11_TEXTURE_ADDRESS_MODE AddressW;
//		FLOAT MipLODBias;
//		UINT MaxAnisotropy;
//		D3D11_COMPARISON_FUNC ComparisonFunc;
//		FLOAT BorderColor[4];
//		FLOAT MinLOD;
//		FLOAT MaxLOD;

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

	m_pGameInstance->Get_LevelMgr()->Request_ChangeLevel("Logo_Level"); //로고 레벨로 시작!
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
	m_pGameInstance->Get_LevelMgr()->Register_Level("Loading_Level", []()->CLevel* {return CLoadingLevel::Create("Loading_Level"); });
	m_pGameInstance->Get_LevelMgr()->Register_Level("Logo_Level", []()->CLevel* {return CLogoLevel::Create("Logo_Level"); });

	/*이후로 계속*/

	m_pGameInstance->Get_LevelMgr()->Set_LoadingLevel("Loading_Level"); //로딩 레벨을 설정함
	m_pGameInstance->Notify_LevelSet(); //레벨 세팅 끝났음을 알림 (게임 인스턴스에게)
} //게임 진행되기 전에 레벨 모두 등록시켜두고. 로딩 레벨 있으면 설정해두고. 그다음에 레벨 설정 끝났다고 인스탄스한테 알려주면
//얘가 레벨과 관련된 시스템 설정을 함께 돌리는거지.  난 로딩도 이렇게 해./ 노티파이는 클라이언트 단에서. 싱크 투는 엔진 단에서.

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

