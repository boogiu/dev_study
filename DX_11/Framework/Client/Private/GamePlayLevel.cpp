#include "Client_Defines.h"
#include "GamePlayLevel.h"
#include "GameInstance.h"
#include "IRenderService.h"
#include "IResourceService.h"
#include "IFontService.h"

#include "ClientHelper.h"
#include "Builder.h"

#include "Player.h"
#include "NpcRco.h"

#include "Target_Camera.h"
#include "Free_Camera.h"
#include "SunCam.h"
#include "Camera.h"

#include "ToolItem.h"
#include "HairParts.h"
#include "HairCapParts.h"
#include "ClothParts.h"
#include "PlayerPart_Hand.h"
#include "MapLoader.h"
#include "AutoTile.h"
#include "Layer.h"
#include "FieldHole.h"

#include "Player_Inventory.h"
#include "Item_Object.h"

#include "UI_InvenSlot.h"
#include "UI_Cursor.h"
#include "UI_ItemIcon.h"
#include "UI_ItemText.h"
#include "TexturePanel.h"
#include "SelectPanel.h"
#include "UI_Text.h"
#include "UI_EventMsg.h"
#include "UI_NameTag.h"
#include "UI_TalkingMsg.h"

#include "Insect_Object.h"

#include "ItemSpawner.h"
#include "InsectSpawner.h"
#include "UI_Responcer.h"
#include "EventSystem.h"

CGamePlayLevel::CGamePlayLevel(const string& LevelKey)
	:CLevel{ LevelKey },
	m_pGameInstance(CGameInstance::GetInstance())
{

	Safe_AddRef(m_pGameInstance);
	m_pProtoManager = m_pGameInstance->Get_PrototypeMgr();
	m_pObjectManager = m_pGameInstance->Get_ObjectMgr();

}

HRESULT CGamePlayLevel::Initialize()
{
	
	Add_LevelObject<CItemSpawner>()->Read_ItemData(L"../../Resources/Data/ItemData.json");
	Add_LevelObject<CInsectSpawner>()->Link_ItemSpawner(Get_LevelObject<CItemSpawner>());

	CGameInstance::GetInstance()->Get_FontSystem()->Add_Font("Sindy", TEXT("../../Resources/Font/Sindy.spritefont"));
	Add_LevelObject<CUI_Responcer>();
	Add_LevelObject<CEventSystem>();

	CMapLoader::Load_ModelData();
	CMapLoader::Load_MapData("../../Resources/Data/MapData.dat", { "GamePlay_Level", "Field_Layer" });

	return S_OK;
}

HRESULT CGamePlayLevel::Awake()
{

	CGameObject* pPlayer = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_Player" }).Position({ 550,0,550 }).Build("Player");
	CGameObject* pRco = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_NpcRco" }).Position({ 560,0,650 }).Build("nPlayer");
	Get_LevelObject<CInsectSpawner>()->Read_InsectData(L"../../Resources/Data/InsectData.json");
	Get_LevelObject<CInsectSpawner>()->Set_Target(pPlayer);

	CGameObject* pFreeCamera = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_FreeCamera" })
		.Camera({ (float)Client::g_iWinSizeX / Client::g_iWinSizeY })
		.Position({ 550,10,550 })
		.Build("Free_Cam");

	CAMERA_DESC desc;
	desc.fAspect = (float)Client::g_iWinSizeX / Client::g_iWinSizeY;
	desc.fFar =1500;
	desc.fNear = 1.f;
	desc.fFov = 90.f;

	CGameObject* pSunCamera = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_Sun" })
		.Camera(desc)
		.Position({ 750,150,750 })
		.Build("Sun");

	m_pObjectManager->Add_Object(pPlayer, { "GamePlay_Level", "Player_Layer" });
	m_pObjectManager->Add_Object(pRco, { "GamePlay_Level", "NonPlayer_Layer" });
	m_pObjectManager->Add_Object(pFreeCamera, { "GamePlay_Level", "Camera_Layer" });
	m_pObjectManager->Add_Object(pSunCamera, { "GamePlay_Level", "Camera_Layer" });
	m_pObjectManager->Add_Object(Get_LevelObject<CUI_Responcer>(), { "GamePlay_Level", "UI_Layer" });
	m_pObjectManager->Add_Object(Get_LevelObject<CInsectSpawner>(), { "GamePlay_Level", "Spawner_Layer" });
	//CGameInstance::GetInstance()->Get_CameraMgr()->Set_MainCam(pFreeCamera->Get_Component<CCamera>());
	//CGameInstance::GetInstance()->Get_CameraMgr()->Set_ShadowCam(pSunCamera->Get_Component<CCamera>());

	return S_OK;
}

void CGamePlayLevel::Update()
{
}

HRESULT CGamePlayLevel::Render()
{
	SetWindowText(g_hWnd, TEXT("GamePlayLevel."));
	return S_OK;
}

HRESULT CGamePlayLevel::Render(ID3D11DeviceContext* pContext)
{
	return S_OK;
}

void CGamePlayLevel::PreLoad_Level()
{
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	auto pRenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	/*PlayerShader*/
	pRcsMgr->Add_ResourcePath("PlayerShader.hlsl", "../Bin/ShaderFiles/PlayerShader.hlsl");

	/*Add Palette*/
	pRcsMgr->Add_ResourcePath("mGrass_Grd.dds", "../../Resources/Palette/mGrass_Grd.dds");
	pRcsMgr->Add_ResourcePath("mGrass_GrdEdge.dds", "../../Resources/Palette/mGrass_GrdEdge.dds");
	pRcsMgr->Add_ResourcePath("mGrass_Mix.dds", "../../Resources/Palette/mGrass_Mix.dds");

	pRenderSys->Add_Palette("g_PaletteTexture", pRcsMgr->Load_Texture(G_GlobalLevelKey, "mGrass_Grd.dds"));
	pRenderSys->Add_Palette("g_PaletteEdgeTexture", pRcsMgr->Load_Texture(G_GlobalLevelKey, "mGrass_GrdEdge.dds"));
	pRenderSys->Add_Palette("g_MaskTexture", pRcsMgr->Load_Texture(G_GlobalLevelKey, "mGrass_Mix.dds"));
	ClientHelper::Add_TexturePathFromDirectory("../../Resources/Palette");

	/*Player Model Path*/
	ClientHelper::Add_ModelPathFromDirectory("../../Resources/Models/Player");
	ClientHelper::Add_MaterialPathFromDirectory("../../Resources/Models/Player");

	/*Player Anim Path*/
	ClientHelper::Add_AnimPathFromDirectory("../../Resources/Models/Player/Animations/Movement","Player");
	ClientHelper::Add_AnimPathFromDirectory("../../Resources/Models/Player/Animations/Interaction", "Player");
	ClientHelper::Add_AnimPathFromDirectory("../../Resources/Models/Player/Animations/Base", "Player");
	ClientHelper::Add_AnimPathFromDirectory("../../Resources/Models/Player/Animations/Transfer", "Player");

	/*NonPlayer Model Path*/
	ClientHelper::Add_ModelPathFromDirectory("../../Resources/Models/NonPlayer/Racoon");
	ClientHelper::Add_MaterialPathFromDirectory("../../Resources/Models/NonPlayer/Racoon");
	ClientHelper::Add_AnimPathFromDirectory("../../Resources/Models/NonPlayer/Animations", "NPC");

	/*Tiles  Path*/
	ClientHelper::Add_ModelPathFromDirectory("../../Resources/Models/FieldRoad");
	ClientHelper::Add_MaterialPathFromDirectory("../../Resources/Models/FieldRoad");

	/*FieldOuts  Path*/
	ClientHelper::Add_ModelPathFromDirectory("../../Resources/Models/FieldOut");
	ClientHelper::Add_MaterialPathFromDirectory("../../Resources/Models/FieldOut");

	/*Structure  Path*/
	ClientHelper::Add_ModelPathFromDirectory("../../Resources/Models/Structure");
	ClientHelper::Add_MaterialPathFromDirectory("../../Resources/Models/Structure");

	/*FieldUnit Path*/
	ClientHelper::Add_ModelPathFromDirectory("../../Resources/Models/FieldUnit");
	ClientHelper::Add_MaterialPathFromDirectory("../../Resources/Models/FieldUnit");
	ClientHelper::Add_AnimPathFromDirectory("../../Resources/Models/FieldUnitAnim/PltTreeOakAnim","OakTree");

	/*Insect  Path*/
	ClientHelper::Add_ModelPathFromDirectory("../../Resources/Models/Insect");
	ClientHelper::Add_MaterialPathFromDirectory("../../Resources/Models/Insect");
	ClientHelper::Add_AnimPathFromDirectory("../../Resources/Models/Insect/ButterFly","InsectAgehacho");
	ClientHelper::Add_AnimPathFromDirectory("../../Resources/Models/Insect/DragonFly","InsectAkiakane");

	/*Texture Path*/
	ClientHelper::Add_TexturePathFromDirectory("../../Resources/UI");
	ClientHelper::Add_TexturePathFromDirectory("../../Resources/Models/MenuLayout");
	ClientHelper::Add_TexturePathFromDirectory("../../Resources/Models/Player/Top/YShirsL/Work");
	ClientHelper::Add_TexturePathFromDirectory("../../Resources/Models/Player/Bottom/Normal/Sweat");

	/*Object_Prototype*/
	auto pProtoMgr = CGameInstance::GetInstance()->Get_PrototypeMgr();
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_Player", CPlayer::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_TargetCamera", CTarget_Camera::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_FreeCamera", CFree_Camera::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_Sun", CSunCam::Create());

	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_PlayerTool", CToolItem::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_PlayerPart_Hand", CPlayerPart_Hand::Create());


	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_HairParts", CHairParts::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_HairCapParts", CHairCapParts::Create());

	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_ClothParts", CClothParts::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_AutoTile", CAutoTile::Create());

	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_FieldHole", CFieldHole::Create());

	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_DropItem", CItem_Object::Create());
	//pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_ItemFruit", CItem_Fruit::Create());
	//pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_ItemStone", CItem_Stone::Create());


	/*UI_ZONE*/
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameUI_PlayerInventory", CPlayer_Inventory::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_UI_InvenSlot", CUI_InvenSlot::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_UI_ItemIcon", CUI_ItemIcon::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_UI_ItemText", CUI_ItemText::Create());

	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_UI_Cursor", CUI_Cursor::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_UI_TexturePanel", CTexturePanel::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_UI_SelectPanel", CSelectPanel::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_UI_BaseText", CUI_Text::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_UI_NameTag", CUI_NameTag::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_UI_EventMsg", CUI_EventMsg::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_UI_TalkingMsg", CUI_TalkingMsg::Create());

	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_Insect_Object", CInsect_Object::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_GameObject_NpcRco", CNpcRco::Create());
}

CGamePlayLevel* CGamePlayLevel::Create(const string& LevelKey)
{
	CGamePlayLevel* instance = new CGamePlayLevel(LevelKey);
	if (FAILED(instance->Initialize())) {
		MSG_BOX("GamePlay level Create Failed");
		Safe_Release(instance);
	}
	return instance;
}

void CGamePlayLevel::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
}
