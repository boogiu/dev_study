#include "Editor_Defines.h"
#include "Engine_Defines.h"
#include "EditorMain.h"

#include "GameInstance.h"
#include "IGUIService.h"
#include "ILevelService.h"

#include "EditorLevel.h"
#include "EditorSystem.h"

CEditorMain::CEditorMain()
{
}

CEditorMain::~CEditorMain()
{
}

HRESULT CEditorMain::Initialize()
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

	ImGui::SetCurrentContext(m_pGameInstance->Get_GUISystem()->GetEngineImGuiContext());

	Set_Levels();
	m_pGameInstance->Get_LevelMgr()->Request_ChangeLevel("Editor_Level", false);
	CEditorLevel::PreLoad_Level();
	

	m_pSystem = CEditorSystem::GetInstance();
	if (m_pSystem)
		m_pSystem->Initialize();
	return S_OK;
}

void CEditorMain::Update(_float dt)
{
	m_pGameInstance->Update_Engine(dt);
	m_pSystem->Update(dt);
}

HRESULT CEditorMain::Render()
{
	_float4 color = { 0.1f,0.3f,0.5f,1.f };
	m_pGameInstance->Draw_Begin(&color);
	m_pGameInstance->Draw();
	m_pGameInstance->Draw_End();
	return S_OK;
}

void CEditorMain::Set_Levels()
{
	m_pGameInstance->Get_LevelMgr()->Register_Level("Editor_Level", []()->CLevel* {return CEditorLevel::Create("Editor_Level"); });
	m_pGameInstance->Notify_LevelSet(); //레벨 세팅 끝났음을 알림 (게임 인스턴스에게)
}

CEditorMain* CEditorMain::Create()
{
	CEditorMain* instance = new CEditorMain();

	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

void CEditorMain::Free()
{
	__super::Free();
	Safe_Release(m_pSystem);
	m_pGameInstance->Release_Engine();
	m_pGameInstance->DestroyInstance();
}
