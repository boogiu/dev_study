#include "Client_Defines.h"
#include "LogoLevel.h"
#include "GameInstance.h"
#include "IObjectService.h"
#include "GameObject.h"

CLogoLevel::CLogoLevel()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CLogoLevel::Initialize()
{
	IObjectService* pObjMgr= m_pGameInstance->Get_ObjectMgr();

	pObjMgr->Create_Object({ "Logo_Level","Proto_GameObject_Background" })
		.Add_Layer({ "Logo_Level", "Layer_BackGround" })
		.With_Transform({ 10.f,10.f })
		.Build("Instance");

	return S_OK;
}

void CLogoLevel::Update()
{
}

HRESULT CLogoLevel::Render()
{
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
	return S_OK;
}

CLogoLevel* CLogoLevel::Create()
{
	CLogoLevel* instance = new CLogoLevel;
	if (FAILED(instance->Initialize())) {
		MSG_BOX("LOGO level Create Failed");
		Safe_Release(instance);
	}

	return instance;
}

void CLogoLevel::Free()
{
	m_pGameInstance->DestroyInstance();
	__super::Free();
}
