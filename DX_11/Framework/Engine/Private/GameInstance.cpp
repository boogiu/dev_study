#include "Engine_Defines.h"
#include "GameInstance.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "AudioDevice.h"
#include "LevelMgr.h"
#include "GraphicDevice.h"
#include "PrototypeMgr.h"
#include "ObjectMgr.h"
#include "ResourceMgr.h"
#include "GUISystem.h"
#include "RenderSystem.h"
#include "CameraMgr.h"
#include "UI_Manager.h"
#include "LightMgr.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

CGameInstance::~CGameInstance()
{
}

_bool CGameInstance::Init_Engine(const ENGINE_DESC& engine)
{
	m_ClientRect.right = engine.iWinSizeX;
	m_ClientRect.bottom = engine.iWinSizeY;

	m_pGraphicDevice = CGraphicDevice::Create(engine, &m_pDevice, &m_pDeviceContext);
	m_pTimeManager = CTimeMgr::Create();
	m_pInputDevice = CInputMgr::Create(engine.hWnd);
	m_pSoundDevice = CAudioDevice::Create();
	m_pLevelManager = CLevelMgr::Create();
	m_pPrototypeManager = CPrototypeMgr::Create();
	m_pObjectManager = CObjectMgr::Create();
	m_pResourceManager = CResourceMgr::Create(m_pDevice, m_pDeviceContext);
	m_RenderSystem = CRenderSystem::Create(m_pDevice, m_pDeviceContext);
	m_CameraManager = CCameraMgr::Create();
	m_UIManager = CUI_Manager::Create();
	m_LightService = CLightMgr::Create();

#if defined _DEBUG
	m_pGuiSystem = CGUISystem::Create(engine, m_pDevice, m_pDeviceContext);
#endif

	Notify_LevelSet();
	return TRUE;
}

void CGameInstance::Notify_LevelSet()
{
	m_pPrototypeManager->Sync_To_Level();
	m_pObjectManager->Sync_To_Level();
	m_pResourceManager->Sync_To_Level();
	m_UIManager->Sync_To_Level();
}

void CGameInstance::Clear_LevelResource(const string& levelKey)
{
	if (levelKey.empty()) return;

	m_pPrototypeManager->Clear(levelKey);
	m_pResourceManager->Clear_Resource(levelKey);
	m_pObjectManager->Clear(levelKey);
	m_UIManager->Clear(levelKey);
}


void CGameInstance::Update_Engine(_float dt)
{
	m_pObjectManager->Priority_Update(dt);
	m_UIManager->Priority_Update(dt);

	m_pInputDevice->Update();
	m_pLevelManager->Update(dt);
	m_CameraManager->Update(dt);
	m_pObjectManager->Update(dt);
	m_UIManager->Update(dt);
	m_pSoundDevice->Update();

#if defined _DEBUG
	m_pGuiSystem->Update(dt);
#endif

	m_pObjectManager->Late_Update(dt);
	m_UIManager->Late_Update(dt);

	m_pObjectManager->Engine_Update(dt);
	m_UIManager->Engine_Update(dt);
}

void CGameInstance::Release_Engine()
{

	/*Managers*/
	Safe_Release(m_pTimeManager);
	
	Safe_Release(m_pLevelManager);
	Safe_Release(m_pPrototypeManager);
	Safe_Release(m_pObjectManager);
	Safe_Release(m_pResourceManager);
	Safe_Release(m_RenderSystem);
	Safe_Release(m_CameraManager);
	Safe_Release(m_pGuiSystem);
	Safe_Release(m_UIManager);
	Safe_Release(m_LightService);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pInputDevice);
	Safe_Release(m_pSoundDevice);

	DestroyInstance();
}

_bool CGameInstance::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#if defined _DEBUG
	if (m_pGuiSystem)
		m_pGuiSystem->Set_ProcHandler(hWnd, message, wParam, lParam);
#endif
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return true;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		break;
	case WM_INPUT:
		m_pInputDevice->Process_Input(lParam);
		break;
	case WM_SIZE:
		GetClientRect(hWnd, &m_ClientRect);
		break;
	default:
		break;
	}

	return false;
}

HRESULT CGameInstance::Draw_Begin(_float4* pColor)
{
	m_pGraphicDevice->Clear_BackBuffer_View(pColor);
	m_pGraphicDevice->Clear_DepthStencil_View();
	return S_OK;
}

HRESULT CGameInstance::Draw()
{
	m_pLevelManager->Render();
	m_RenderSystem->Render();

#if defined _DEBUG
	m_pGuiSystem->Render_GUI();
#endif
	return S_OK;
}

HRESULT CGameInstance::Draw_End()
{
	m_pGraphicDevice->Present();
	return S_OK;
}

void CGameInstance::Free()
{
	__super::Free();
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
