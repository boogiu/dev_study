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
	m_pGraphicService = m_ServiceHub.Add_Service<IGraphicService, CGraphicDevice>(engine,&m_pDevice, &m_pDeviceContext);
	Safe_AddRef(m_pGraphicService);

	m_ServiceHub.Add_Service<ITimeService, CTimeMgr>();
	m_ServiceHub.Add_Service<IInputService, CInputMgr>(engine.hWnd);
	m_ServiceHub.Add_Service<ISoundService, CSoundMgr>();
	m_ServiceHub.Add_Service<ILevelService, CLevelMgr>();

	return TRUE;
}

_bool CGameInstance::InitDirectX()
{
	return TRUE;
}

void CGameInstance::Update_Engine(_float dt)
{
	m_ServiceHub.Get_Service<IInputService>()->Update();
	m_ServiceHub.Get_Service<ISoundService>()->Update();
	m_ServiceHub.Get_Service<ILevelService>()->Update(dt);

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
		Get_Service<IInputService>()->Process_Input(lParam);
	}
	
	return false;
}

HRESULT CGameInstance::Draw_Begin(_float4* pColor)
{
	m_pGraphicService->Clear_BackBuffer_View(pColor);
	m_pGraphicService->Clear_DepthStencil_View();
	return S_OK;
}

HRESULT CGameInstance::Draw()
{
	return S_OK;
}

HRESULT CGameInstance::Draw_End()
{
	m_pGraphicService->Present();
	return S_OK;
}

void CGameInstance::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pGraphicService);
	m_ServiceHub.Service_Free();
}
