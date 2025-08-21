#include "Engine_Defines.h"
#include "Engine_Service.h"
#include "GameInstance.h"

#include "TimeMgr.h"
#include "InputMgr.h"
#include "SoundMgr.h"
#include "LevelMgr.h"
#include "GraphicDevice.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

CGameInstance::~CGameInstance()
{
}

_bool CGameInstance::InitSystems(const ENGINE_DESC& engine)
{
	m_pGraphicDevice = CGraphicDevice::Create(engine, &m_pDevice, &m_pDeviceContext);
	Safe_AddRef(m_pGraphicDevice);

	m_pTimeManager = CTimeMgr::Create();
	Safe_AddRef(m_pTimeManager);

	m_pInputDevice= CInputMgr::Create(engine.hWnd);
	Safe_AddRef(m_pInputDevice);

	m_pSoundDevice = CSoundMgr::Create();
	Safe_AddRef(m_pSoundDevice);

	m_pLevelManager = CLevelMgr::Create();
	Safe_AddRef(m_pLevelManager);
	return TRUE;
}

_bool CGameInstance::InitDirectX()
{
	return TRUE;
}

void CGameInstance::Update_Engine(_float dt)
{
	m_pInputDevice->Update();
	m_pSoundDevice->Update();
	m_pLevelManager->Update(dt);
}

_bool CGameInstance::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return true;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);

	case WM_INPUT:
		m_pInputDevice->Process_Input(lParam);
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
	Safe_Release(m_pDevice);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pTimeManager);
	Safe_Release(m_pInputDevice);
	Safe_Release(m_pSoundDevice);
	Safe_Release(m_pLevelManager);
}
