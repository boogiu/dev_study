#include "Client_Defines.h"
#include "LogoLevel.h"
#include "GameObject.h"

#include "GameInstance.h"
#include "IProtoService.h"
#include "IObjectService.h"
#include "IResourceService.h"
#include "ICameraService.h"

#include "BackGround.h"
#include "Free_Camera.h"
#include "Camera.h"
#include "Terrain.h"
#include "Builder.h"
#include "StaticObject.h"

CLogoLevel::CLogoLevel(const string& LevelKey)
	: CLevel{ LevelKey },
	m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CLogoLevel::Initialize()
{
	_uint ID2{};

//Builder::Create_UIObject({ "Logo_Level","Proto_GameObject_Background" })
//	.Add_To_Level("Logo_Level")
//	.Position({ g_iWinSizeX * 0.9f,g_iWinSizeY * 0.9f })
//	.Scale({90,90 })
//	.Build("Main_UI");
//
//Builder::Create_Object({ "Logo_Level", "Proto_GameObject_Terrain" })
//	.Add_To_Layer({ "Logo_Level","Layer_BackGround" })
//	.Position({ 0,10,10 })
//	.Build("Terrain");

	Builder::Create_Object({ "Logo_Level", "Proto_GameObject_Static" })
		.Add_To_Layer({ "Logo_Level","Layer_Object" })
		.Position({ 0,30,10 })
		.Build("StaticObj");

	CGameObject* Camera = Builder::Create_Object({ "Logo_Level","Proto_GameObject_Camera" })
		.Add_To_Layer({ "Logo_Level","Layer_Camera" })
		.Camera({ (float)g_iWinSizeX / g_iWinSizeY })
		.Position({ 0,10,0 })
		.Build("Main_Camera");

	m_pGameInstance->Get_CameraMgr()->Set_MainCam(Camera->Get_Component<CCamera>());
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
	IResourceService* pResourceMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	/*Loading Path*/
	pResourceMgr->Add_ResourcePath( "Default_Tex", "../Bin/Resources/Textures/Terrain/Grass_1.dds");
	pResourceMgr->Add_ResourcePath( "Default_TexUI", "../Bin/Resources/Textures/TestUI.png");
	pResourceMgr->Add_ResourcePath( "Default_Terrain", "../Bin/Resources/Textures/Terrain/Height.bmp");
	pResourceMgr->Add_ResourcePath( "Test_Sound", "../Bin/Resources/Sounds/Test.wav");

	///*Loading Buffer*/ -> 사실상 오브젝트 로드할 때 로드 됨.
	//pResourceMgr->Load_VIBuffer("Logo_Level", "Default_Terrain", BUFFER_TYPE::TERRAIN);
	///*Loading Sound*/
	//pResourceMgr->Load_Sound("Logo_Level", "Test_Sound");

	/*Loading Object*/
	pProtoMgr->Add_ProtoType("Logo_Level", "Proto_GameObject_Background", CBackGround::Create());
	pProtoMgr->Add_ProtoType("Logo_Level", "Proto_GameObject_Camera", CFree_Camera::Create());
	pProtoMgr->Add_ProtoType("Logo_Level", "Proto_GameObject_Terrain", CTerrain::Create());
	pProtoMgr->Add_ProtoType("Logo_Level", "Proto_GameObject_Static", CStaticObject::Create());
}
