#include "Editor_Defines.h"
#include "EditorSystem.h"
#include "GameInstance.h"

#include "IRayService.h"
#include "ICameraService.h"
#include "IObjectService.h"
#include "IProtoService.h"
#include "IResourceService.h"
#include "IRenderService.h"
#include "IGUIService.h"
#include "GUI_Context.h"
#include "IInputService.h"
#include "ITileService.h"

#include "Layer.h"
#include "GridObject.h"

#include "TileObject.h"
#include "FieldOutBlocks.h"

#include "DirectoryPanel.h"
#include "ControlPanel.h"

IMPLEMENT_SINGLETON(CEditorSystem)

CEditorSystem::CEditorSystem()
{
	Initialize();
}


HRESULT CEditorSystem::Initialize()
{
	/*Managers*/
	m_pRayManager = CGameInstance::GetInstance()->Get_RayMgr();
	m_pViewMat = CGameInstance::GetInstance()->Get_CameraMgr()->Get_ViewMatrix();
	m_pProjMat = CGameInstance::GetInstance()->Get_CameraMgr()->Get_ProjMatrix();
	m_pInputService = CGameInstance::GetInstance()->Get_InputDev();
	m_pObjMgr = CGameInstance::GetInstance()->Get_ObjectMgr();
	m_pUIContext = CGameInstance::GetInstance()->Get_GUISystem()->Get_Context();
	m_pRenderSystem = CGameInstance::GetInstance()->Get_RenderSystem();

	/*그리드 오브젝트*/
	IProtoService* pProto = CGameInstance::GetInstance()->Get_PrototypeMgr();
	Execute_TileSystem();

	Create_GUIPanels();
	/*타일 오브젝트 원형*/
	pProto->Add_ProtoType(G_GlobalLevelKey, "Proto_GameObject_Tile", CTileObject::Create());
	pProto->Add_ProtoType(G_GlobalLevelKey, "Proto_GameObject_FieldOut", CFieldOutBlocks::Create());

	/*팔레트 우선 등록*/
	auto pResMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	pResMgr->Add_ResourcePath("mGrass_Grd.dds", "../../Resources/Palette/mGrass_Grd.dds");
	pResMgr->Add_ResourcePath("mGrass_GrdEdge.dds", "../../Resources/Palette/mGrass_GrdEdge.dds");
	pResMgr->Add_ResourcePath("mGrass_Mix.dds", "../../Resources/Palette/mGrass_Mix.dds");
	   
	m_pRenderSystem->Add_Palette("g_PaletteTexture", pResMgr->Load_Texture(G_GlobalLevelKey, "mGrass_Grd.dds"));
	m_pRenderSystem->Add_Palette("g_PaletteEdgeTexture", pResMgr->Load_Texture(G_GlobalLevelKey, "mGrass_GrdEdge.dds"));
	m_pRenderSystem->Add_Palette("g_MaskTexture", pResMgr->Load_Texture(G_GlobalLevelKey, "mGrass_Mix.dds"));

	return S_OK;
}

void CEditorSystem::Update(_float dt)
{
	Create_Ray();
	DragDrop_Object();

	m_pRayManager->Register_Ray(&m_tRay);
}


void CEditorSystem::Execute_TileSystem()
{
	TILESYSTEM_INFO info = {};

	info.iSizeXPerTile = 10.f;
	info.iSizeYPerTile = 15;
	info.iSizeZPerTile = 10.f;

	info.iTileCountX = 16*6*2;
	info.iTileCountY = 4;/*0~4 -> 5칸*/
	info.iTileCountZ = 16*6 * 2;

	info.OriginPoint = { 0,0,0,1 };

	/*타일 시스템 구동*/
	CGameInstance::GetInstance()->Excute_TileSystem(info);
	m_pTileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	m_EditorContext.ContextTileInfo = info;

	_float GridXscale = static_cast<_float>(info.iTileCountX * info.iSizeXPerTile);
	_float GridZscale = static_cast<_float>(info.iTileCountZ * info.iSizeZPerTile);
	_float GridYPostion = static_cast<_float>(info.iSizeYPerTile);

	/*그리드 역할의 오브젝트 생성*/
	IProtoService* pProto = CGameInstance::GetInstance()->Get_PrototypeMgr();
	pProto->Add_ProtoType(G_GlobalLevelKey, "Proto_GameObject_Grid", CGridObject::Create());

	CGameObject* pGrid = Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Grid" })
		.Position({ GridXscale / 2, 0, GridZscale / 2 })
		.Scale({ GridXscale ,	1	, GridZscale })
		.Build("Grid");

	m_pObjMgr->Add_Object(pGrid, { G_GlobalLevelKey,"Global_Layer" });
	m_pGrid = dynamic_cast<CGridObject*>(pGrid);
	Safe_AddRef(m_pGrid);
}

void CEditorSystem::Create_GUIPanels()
{
	/*패널 생성*/
	m_pDirectoryPanel = CDirectoryPanel::Create(m_pUIContext);
	m_pControlPanel = CControlPanel::Create(m_pUIContext);
	CGameInstance::GetInstance()->Get_GUISystem()->Register_Panel(m_pDirectoryPanel);
	CGameInstance::GetInstance()->Get_GUISystem()->Register_Panel(m_pControlPanel);
	m_pControlPanel->Set_Grid(m_pGrid);
}

void CEditorSystem::Create_Ray()
{
	GetCursorPos(&m_MousePt);
	ScreenToClient(g_hWnd, &m_MousePt);

	/*마우스 좌표 변환*/
	_vector vMouseOrigin = XMVectorSet(
		static_cast<float>(m_MousePt.x),
		static_cast<float>(m_MousePt.y),
		0.f,
		1.f
	);

	/*마우스 레이 변환*/
	XMVECTOR raySrc = XMVector3Unproject(
		vMouseOrigin,
		0.0f, 0.0f,
		static_cast<float>(MapEditor::g_iWinSizeX),
		static_cast<float>(MapEditor::g_iWinSizeY),
		0.0f, 1.0f,
		XMLoadFloat4x4(m_pProjMat),
		XMLoadFloat4x4(m_pViewMat),
		XMMatrixIdentity()
	);

	/*마우스 목적지 좌표*/
	_vector vMouseDest = XMVectorSet(
		static_cast<float>(m_MousePt.x),
		static_cast<float>(m_MousePt.y),
		1.f,
		1.f
	);

	XMVECTOR rayDest = XMVector3Unproject(
		vMouseDest,
		0.0f, 0.0f,
		static_cast<float>(MapEditor::g_iWinSizeX),
		static_cast<float>(MapEditor::g_iWinSizeY),
		0.0f, 1.0f,
		XMLoadFloat4x4(m_pProjMat),
		XMLoadFloat4x4(m_pViewMat),
		XMMatrixIdentity()
	);

	XMVECTOR rayOrigin = raySrc;
	XMVECTOR rayDir = XMVector3Normalize(rayDest - raySrc);

	XMStoreFloat3(&m_tRay.vRayDirection, rayDir);
	XMStoreFloat3(&m_tRay.vRayOrigin, rayOrigin);
	m_tRay.fMaxDistance = 1550.f;
}

void CEditorSystem::Create_Base()
{
	auto pResMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	pResMgr->Add_ResourcePath("Base_0.model", "../../Resources/Models/FieldRoad/Base/Base_0/Base_0.model");
	pResMgr->Add_ResourcePath("Base_0.mat", "../../Resources/Models/FieldRoad/Base/Base_0/Base_0.mat");
	auto info = m_EditorContext.ContextTileInfo;

	_float3 Pos = {
		info.OriginPoint.x + info.iSizeXPerTile *info.iTileCountX * 0.5f,
		info.OriginPoint.y - 0.1f,
		info.OriginPoint.z + info.iSizeZPerTile * info.iTileCountZ * 0.5f,
	};

	/*베이스 타일을 생성*/
	CGameObject* pObject =
		Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Tile" })
		.Position(Pos)
		.Scale({ 16 * 7,10, 16 * 6 })
		.Build("Base_Plane", &m_BaseTileID);

	m_pObjMgr->Add_Object(pObject, { "Editor_Level","Base_Layer" });
	dynamic_cast<CTileObject*>(pObject)->Link_Data("Base_0",true);

	return;
}

void CEditorSystem::DragDrop_Object()
{
	if (nullptr == m_EditorContext.pEditingObject) return;
	CGameObject* obj = m_EditorContext.pEditingObject;


	if (m_pInputService->Mouse_Hold(MOUSE_BTN::LB)) {
		obj->Get_Component<CTransform>()->Set_Pos({ m_pGrid->Get_HitPos() });
	}

	if (m_pInputService->Mouse_Away(MOUSE_BTN::LB)) {
		TILE_INDEX index = m_pGrid->Get_HitIndex();
		switch (m_EditorContext.eType)
		{
		case TILE: {
			CTileObject* object = dynamic_cast<CTileObject*>(obj);

			if (object)
				object->Object_OnGrid(index);
		}

			break;
		case FIELDOUT: {
			CFieldOutBlocks* object = dynamic_cast<CFieldOutBlocks*>(obj);
			if (object)
				object->Object_OnGrid(index);
		}
			break;
		default:
			break;
		}
		

		m_pDirectoryPanel->Notify_CreateComplete();
		m_EditorContext.pEditingObject = nullptr;
	}
}

HRESULT CEditorSystem::Save_Blocks()
{

	string path = "../../Resources/Data/MapData.dat";
	filesystem::path directory(path);

	ofstream ofs(path.c_str(), ios::binary);
	if (!ofs.is_open())
		return E_FAIL;

	/*타일 전체 정보 저장*/
	TILESYSTEM_INFO systemInfo =m_EditorContext.ContextTileInfo;
	ofs.write(reinterpret_cast<char*>(&systemInfo), sizeof(TILESYSTEM_INFO));

	/*1. Base Tile*/
	auto iter = m_pObjMgr->Get_LevelLayer("Editor_Level").find("Base_Layer");
	if (iter == m_pObjMgr->Get_LevelLayer("Editor_Level").end())
		return E_FAIL;
	auto& ObjectLayer = iter->second;
	CGameObject* pObject = ObjectLayer->Find_ObjectByID(m_BaseTileID);
	CTileObject* pObj = dynamic_cast<CTileObject*>(pObject);

	pObj->Save_Blocks(ofs, true);


	ofs.close();

	return S_OK;
}


HRESULT CEditorSystem::Create_Tile(const string& folderName)
{
	if (m_EditorContext.pEditingObject != nullptr) return E_FAIL;

	/*타일을 생성*/
	CGameObject* pObject =
		Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Tile" })
		.Position({ 0,0,0 })
		.Scale({ 1,1,1 })
		.Build(folderName);
	m_pObjMgr->Add_Object(pObject, { "Editor_Level","Block_Layer" });

	dynamic_cast<CTileObject*>(pObject)->Link_Data(folderName);
	m_EditorContext.pEditingObject = pObject;
	m_EditorContext.eType = TILE;
	return S_OK;
}

HRESULT CEditorSystem::Create_FieldOut(const string& folderName)
{
	if (m_EditorContext.pEditingObject != nullptr) return E_FAIL;

	/*타일을 생성*/
	CGameObject* pObject =
		Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_FieldOut" })
		.Position({ 0,0,0 })
		.Scale({ 1,1,1 })
		.Build(folderName);

	m_pObjMgr->Add_Object(pObject, { "Editor_Level","FieldOut_Layer" });

	dynamic_cast<CFieldOutBlocks*>(pObject)->Link_Data(folderName);
	m_EditorContext.pEditingObject = pObject;
	m_EditorContext.eType = FIELDOUT;
	return S_OK;
}

HRESULT CEditorSystem::Delete_Object(CGameObject* pObject)
{
	if(pObject)
		m_pObjMgr->Remove_Object(pObject);

	return S_OK;
}

void CEditorSystem::Free()
{
	__super::Free();
	Safe_Release(m_pGrid);
	Safe_Release(m_pDirectoryPanel);
}
