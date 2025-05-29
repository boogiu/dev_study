#include "pch.h"
#include "CMainApp.h"
#include "Engine_Define.h"

CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
	return S_OK;
}

int CMainApp::Update_MainApp(const float& fTimeDelta)
{
	return 0;
}

void CMainApp::LateUpdate_MainApp(const float& fTimeDelta)
{
}

void CMainApp::Render_MainApp()
{
}

CMainApp* CMainApp::Create()
{
	CMainApp* instance = new CMainApp;
	if (FAILED(instance->Ready_MainApp())) {
		Engine::Safe_Release(instance);
		return nullptr;
	}
	return instance;

}

void CMainApp::Free()
{
}
