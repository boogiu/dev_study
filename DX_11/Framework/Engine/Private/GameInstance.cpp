#include "Engine_Defines.h"
#include "GameInstance.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "SoundDevice.h"
#include "LevelMgr.h"
#include "GraphicDevice.h"
#include "PrototypeMgr.h"
#include "ObjectMgr.h"
#include "ResourceMgr.h"
#include "GUISystem.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

CGameInstance::~CGameInstance()
{
}

_bool CGameInstance::Init_Engine(const ENGINE_DESC& engine)
{
	m_pGraphicDevice = CGraphicDevice::Create(engine, &m_pDevice, &m_pDeviceContext);
	m_pTimeManager = CTimeMgr::Create();
	m_pInputDevice= CInputMgr::Create(engine.hWnd);
	m_pSoundDevice = CSoundDevice::Create();
	m_pLevelManager = CLevelMgr::Create();
	m_pPrototypeManager = CPrototypeMgr::Create();
	m_pObjectManager = CObjectMgr::Create();
	m_pResourceManager = CResourceMgr::Create(m_pDevice, m_pDeviceContext);
	m_pGuiSystem = CGUISystem::Create(engine,m_pDevice, m_pDeviceContext);

	Notify_LevelSet();
	m_pResourceManager->Load_InitialResource();
	return TRUE;
}

void CGameInstance::Notify_LevelSet()
{
	m_pPrototypeManager->Sync_To_Level();
	m_pObjectManager->Sync_To_Level();
	m_pResourceManager->Sync_To_Level();
}


void CGameInstance::Update_Engine(_float dt)
{
	m_pObjectManager->Priority_Update(dt);
	
	m_pObjectManager->Update(dt);

	m_pInputDevice->Update();
	m_pSoundDevice->Update();
	m_pLevelManager->Update(dt);
	m_pGuiSystem->Update(dt);
	m_pObjectManager->Late_Update(dt);
}

void CGameInstance::Release_Engine()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pTimeManager);
	Safe_Release(m_pInputDevice);
	Safe_Release(m_pSoundDevice);
	Safe_Release(m_pLevelManager);
	Safe_Release(m_pPrototypeManager);
	Safe_Release(m_pObjectManager);
	Safe_Release(m_pResourceManager);
	Safe_Release(m_pGuiSystem);
	DestroyInstance();
}

_bool CGameInstance::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(m_pGuiSystem)
		m_pGuiSystem->Set_ProcHandler(hWnd, message, wParam, lParam);

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
	default:
		break;
	}
	
	return false;
}

HRESULT CGameInstance::Draw_Begin(_float4* pColor)
{
	m_pGraphicDevice->Clear_BackBuffer_View(pColor);
	m_pGraphicDevice->Clear_DepthStencil_View();
	m_pGuiSystem->GUI_Begin();
	return S_OK;
}

HRESULT CGameInstance::Draw()
{
	m_pLevelManager->Render();
	m_pGuiSystem->Render_GUI();
	return S_OK;
}

HRESULT CGameInstance::Draw_End()
{
	m_pGuiSystem->GUI_End();
	m_pGraphicDevice->Present();
	return S_OK;
}

void CGameInstance::Free()
{
	__super::Free();
}
