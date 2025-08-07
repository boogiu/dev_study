#include "pch.h"
#include "CMainApp.h"

#include "CTimeMgr.h"
#include "CFrameMgr.h"
#include "CCameraMgr.h"
#include "CRenderMgr.h"
#include "CLightMgr.h"
#include "CResourceMgr.h"
#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CThreadMgr.h"

#include "CScene.h"
#include "CTestScene.h"


CMainApp::CMainApp()
	:m_pDeviceClass(nullptr)
	,m_pGraphicDev(nullptr)
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
	if (FAILED(CGraphicDev::GetInstance()->Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass)))
		return E_FAIL;

	m_pDeviceClass->AddRef();

	m_pGraphicDev = m_pDeviceClass->Get_GraphicDev();
	m_pGraphicDev->AddRef();

	Ready_Manager();
	Ready_Scene();

	return S_OK;
}

HRESULT CMainApp::Ready_Manager()
{
	if (FAILED(CRenderMgr::GetInstance()->Ready_RenderMgr(m_pGraphicDev)))
		return E_FAIL;

	if (FAILED(CLightMgr::GetInstance()->Ready_Light(m_pGraphicDev)))
		return E_FAIL;

	if (FAILED(CResourceMgr::GetInstance()->Ready_Resoource()))
		return E_FAIL;

	if (FAILED(CInputMgr::GetInstance()->Ready_InputDev(g_HInst, g_hWnd)))
		return E_FAIL;

	if (FAILED(CSceneMgr::GetInstance()->Ready_Scene()))
		return E_FAIL;

	if (FAILED(CThreadMgr::GetInstance()->Ready_Thread()))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Ready_Scene()
{
	CScene* tmp;

	tmp = CTestScene::Create();

	if (FAILED(CSceneMgr::GetInstance()->Add_Scene(L"Test_Scene", tmp))) 
			MessageBoxW(0, L"테스트씬 로드 실패", L"error", MB_OK);


	CSceneMgr::GetInstance()->Change_Scene(L"Test_Scene");
}

int CMainApp::Update_MainApp( _float&fTimeDelta)
{
	CInputMgr::GetInstance()->Update_InputDev();
	CLightMgr::GetInstance()->Update_Light(fTimeDelta);
	CSceneMgr::GetInstance()->Update_Scene(fTimeDelta);

	return 0;
}

void CMainApp::LateUpdate_MainApp( _float&fTimeDelta)
{
	CInputMgr::GetInstance()->LateUpdate_InputDev();
	CCameraMgr::GetInstance()->Apply_Camera(m_pGraphicDev);
	CSceneMgr::GetInstance()->LateUpdate_Scene(fTimeDelta);

}

void CMainApp::Render_MainApp()
{
	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	CRenderMgr::GetInstance()->Render(m_pGraphicDev);

	m_pDeviceClass->Render_End();
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();
	if (FAILED(pInstance->Ready_MainApp())) {
		Engine::Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);

	CTimeMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CRenderMgr::GetInstance()->DestroyInstance();
	CCameraMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	CResourceMgr::GetInstance()->DestroyInstance();
	CInputMgr::GetInstance()->DestroyInstance();
	CThreadMgr::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance(); 
}
