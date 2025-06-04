#include "pch.h"
#include "CMainApp.h"
#include "CTimeMgr.h"
#include "CFrameMgr.h"
#include "CCameraMgr.h"
#include "CRenderMgr.h"
#include "CLightMgr.h"
#include "CTextureMgr.h"

#include "CGameObject.h"
#include "CCamera.h"

#include "CTestObj.h"
#include "CTestCam.h"
#include "CTestLigh.h"

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

	if (FAILED(CRenderMgr::GetInstance()->Ready_RenderMgr(m_pGraphicDev)))
		return E_FAIL;

	if (FAILED(CLightMgr::GetInstance()->Ready_Light(m_pGraphicDev)))
		return E_FAIL;
	
	if (FAILED(CTextureMgr::GetInstance()->Ready_Texture(m_pGraphicDev,"../../Images")))
		return E_FAIL;

	pCam = CTestCam::Create();
	pTest = CTestObj::Create();
	//pLight = CTestLigh::Create();

	pCam->Set_Target(pTest);
	CCameraMgr::GetInstance()->Set_ViewTarget(pCam->Get_Component<CCamera>());
	return S_OK;
}

int CMainApp::Update_MainApp(const float& fTimeDelta)
{
	if (pTest) pTest->Update_GameObject(fTimeDelta);
	if (pCam) pCam->Update_GameObject(fTimeDelta);
	//if (pLight) pLight->Update_GameObject(fTimeDelta);

	return 0;
}

void CMainApp::LateUpdate_MainApp(const float& fTimeDelta)
{
	if (pTest) pTest->LateUpdate_GameObject(fTimeDelta);
	if (pCam) pCam->LateUpdate_GameObject(fTimeDelta);
	//if (pLight) pLight->LateUpdate_GameObject(fTimeDelta);
}

void CMainApp::Render_MainApp()
{
	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	CLightMgr::GetInstance()->Set_Light(m_pGraphicDev);
	CCameraMgr::GetInstance()->Apply_Camera(m_pGraphicDev);
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

	Safe_Release(pTest);
	Safe_Release(pCam);

	CGraphicDev::GetInstance()->DestroyInstance();
	CTimeMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
}
