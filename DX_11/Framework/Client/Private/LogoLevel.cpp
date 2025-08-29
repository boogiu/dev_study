#include "Client_Defines.h"
#include "LogoLevel.h"
#include "GameObject.h"

#include "GameInstance.h"
#include "IProtoService.h"
#include "IObjectService.h"
#include "BackGround.h"

CLogoLevel::CLogoLevel(const string& LevelKey)
	: CLevel{ LevelKey },
	m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CLogoLevel::Initialize()
{
	
		IObjectService* pObjMgr= m_pGameInstance->Get_ObjectMgr();

		CGameObject* obk = pObjMgr->Create_Object({ "Logo_Level","Proto_GameObject_Background" }) //어디서 꺼내냐
		.Add_Layer({ "Logo_Level", "Layer_BackGround" }) //어디로 넣냐 ->기존에 프로토에서 꺼내서 레이어로
		.With_Transform() //뭘 채우냐
		.Set_Position({0.f,0.f,0.f})
		.Set_Rotate({10.f, 10.f,10.f})
		.Build("Instance"); //객체명은 뭐로 할거냐.

		pObjMgr->Create_Object({ "Logo_Level","Proto_GameObject_Background" }) //어디서 꺼내냐
			.Add_Layer({ "Logo_Level", "Layer_BackGround" }) //어디로 넣냐 ->기존에 프로토에서 꺼내서 레이어로
			.With_Transform() //뭘 채우냐
			.Set_Position({ 0.f,0.f,0.f })
			.Set_Rotate({ 10.f, 10.f,10.f })
			.Build("Instance2"); //객체명은 뭐로 할거냐.

		CTransform* m_pTransform = obk->Get_Component<CTransform>();
		m_pTransform->Translate(m_pTransform->Dir(STATE::UP)*10);

		m_pTransform->Get_WorldMatrix();

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

CLogoLevel* CLogoLevel::Create(const string& LevelKey)
{
	CLogoLevel* instance = new CLogoLevel(LevelKey);
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

void CLogoLevel::PreLoad_Level()
{
	IProtoService* pProtoMgr = CGameInstance::GetInstance()->Get_PrototypeMgr();
	pProtoMgr->Add_ProtoType("Logo_Level", "Proto_GameObject_Background", CBackGround::Create());
}
