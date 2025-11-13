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
#include "MapObject.h"

#include "DirectoryPanel.h"
#include "ControlPanel.h"

IMPLEMENT_SINGLETON(CEditorSystem)

CEditorSystem::CEditorSystem()
{
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

	/* 오브젝트*/
	IProtoService* pProto = CGameInstance::GetInstance()->Get_PrototypeMgr();
	pProto->Add_ProtoType(G_GlobalLevelKey, "Proto_GameObject_Tile", CTileObject::Create());
	CTileObject::PrepareForTile("../../Resources/Models/FieldModel/FieldRoad/");
	pProto->Add_ProtoType(G_GlobalLevelKey, "Proto_GameObject_MapObject", CMapObject::Create());

	/*팔레트 우선 등록*/
	auto pResMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	pResMgr->Add_ResourcePath("mGrass_Grd.dds", "../../Resources/Palette/mGrass_Grd.dds");
	pResMgr->Add_ResourcePath("mGrass_GrdEdge.dds", "../../Resources/Palette/mGrass_GrdEdge.dds");
	pResMgr->Add_ResourcePath("mGrass_Mix.dds", "../../Resources/Palette/mGrass_Mix.dds");

	m_pRenderSystem->Add_Palette("g_PaletteTexture", pResMgr->Load_Texture(G_GlobalLevelKey, "mGrass_Grd.dds"));
	m_pRenderSystem->Add_Palette("g_PaletteEdgeTexture", pResMgr->Load_Texture(G_GlobalLevelKey, "mGrass_GrdEdge.dds"));
	m_pRenderSystem->Add_Palette("g_MaskTexture", pResMgr->Load_Texture(G_GlobalLevelKey, "mGrass_Mix.dds"));

	Execute_TileSystem();

	Create_GUIPanels();

	return S_OK;
}

void CEditorSystem::Update(_float dt)
{
	Create_Ray();
	if (m_EditorContext.eMode == EditObj)
		DragDrop_Object();
	else if (m_EditorContext.eMode == BrushTile)
		Brushing_Tiles();
	else if (m_EditorContext.eMode == TileSystem)
		Editing_TileInfo();

	m_pRayManager->Register_Ray(&m_tRay);
}

void CEditorSystem::Execute_TileSystem()
{
	HRESULT hasFile = Load_MapData();

	/*그리드 역할의 오브젝트 생성*/
	IProtoService* pProto = CGameInstance::GetInstance()->Get_PrototypeMgr();
	pProto->Add_ProtoType(G_GlobalLevelKey, "Proto_GameObject_Grid", CGridObject::Create());

	CGameObject* pGrid = Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Grid" })
		.Position(m_EditorContext.ContextTileInfo.HalfPoint())

		.Scale(m_EditorContext.ContextTileInfo.WorldSize())
		.Build("Grid");

	m_pObjMgr->Add_Object(pGrid, { "Editor_Level","Grid_Layer"});
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

void CEditorSystem::Brushing_Tiles()
{
	if (m_pInputService->Mouse_Down(MOUSE_BTN::LB)) {
		m_GridMinEdge = m_pGrid->Get_MinEdge();
	}

	if (CGameInstance::GetInstance()->Get_GUISystem()->UsingUI()) return;

	if (m_pInputService->Mouse_Away(MOUSE_BTN::LB)) {
		m_GridMaxEdge = m_pGrid->Get_MaxEdge();

		auto indexArea = m_pTileSystem->Get_IndeciesByArea(
			{ m_GridMinEdge.x,m_GridMinEdge.y,m_GridMinEdge.z,1 },
			{ m_GridMaxEdge.x,m_GridMaxEdge.y,m_GridMaxEdge.z,1 }
		);

		for (TILE_INDEX index : indexArea) {
			ConvertMaterial(m_EditorContext.baseType, index);
		}

	}
}

void CEditorSystem::ConvertMaterial(string brushType, TILE_INDEX Index)
{
	_float4 vPos = m_pTileSystem->Get_PositionByIndex(Index, ANCHOR::Center); {
		if (brushType == "Base_0") {
			m_pTileSystem->Set_Material_ID(Index, { 1.f,1.f,0.f,0.f });
			m_pTileSystem->Change_CornerHeight(Index, m_EditorContext.ObjHeight, m_EditorContext.ObjHeight, m_EditorContext.ObjHeight, m_EditorContext.ObjHeight);
		}

		else if ("Cliff") {
			CTileObject::TILE_TYPE_DESC* objDesc = new CTileObject::TILE_TYPE_DESC;
			objDesc->TypeName = brushType;
			objDesc->index = Index;
			CGameObject* pObject = Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Tile" })
				.Position({ 0,15,0 })
				.Scale({ 1,1,1 })
				.Add_ObjDesc(objDesc)
				.Build(brushType);
			m_pObjMgr->Add_Object(pObject, { "Editor_Level","Tile_Layer" });
		}
		else {
			CTileObject::TILE_TYPE_DESC* objDesc = new CTileObject::TILE_TYPE_DESC;
			objDesc->TypeName = brushType;
			objDesc->index = Index;
			CGameObject* pObject = Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Tile" })
				.Position({ 0,0,0 })
				.Scale({ 1,1,1 })
				.Add_ObjDesc(objDesc)
				.Build(brushType);
			m_pObjMgr->Add_Object(pObject, { "Editor_Level","Tile_Layer" });
		}
	}
}

void CEditorSystem::DragDrop_Object()
{
	if (nullptr == m_EditorContext.pEditingObject) return;
	CGameObject* obj = m_EditorContext.pEditingObject;

	TILE_INDEX index = m_pGrid->Get_HitIndex();

	if (m_pInputService->Mouse_Hold(MOUSE_BTN::LB)) {
		_uint Anchor = m_EditorContext.eAnchor;
		_float4 DragPos = m_pTileSystem->Get_PositionByIndex(index, static_cast<ANCHOR>(Anchor));
		obj->Get_Component<CTransform>()->Set_Pos({ DragPos.x,DragPos.y,DragPos.z });
	}

	if (m_pInputService->Mouse_Away(MOUSE_BTN::LB)) {
		CMapObject* object = dynamic_cast<CMapObject*>(obj);
		if (object)
			object->Object_OnGrid(index);

		m_pDirectoryPanel->Notify_CreateComplete();
		m_EditorContext.pEditingObject = nullptr;
	}
}

HRESULT CEditorSystem::Create_MapObject(const string& folderName)
{
	if (m_EditorContext.pEditingObject != nullptr) return E_FAIL;

	CGameObject* pObject = { nullptr };
	pObject = Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_MapObject" })
		.Position({ 0,0,0 })
		.Scale({ 1,1,1 })
		.Build(folderName);
	m_pObjMgr->Add_Object(pObject, { "Editor_Level","MapObject_Layer" });
	dynamic_cast<CMapObject*>(pObject)->Link_Data(folderName);

	m_EditorContext.pEditingObject = pObject;
	return S_OK;
}


void CEditorSystem::Editing_TileInfo()
{
	if (m_pInputService->Mouse_Down(MOUSE_BTN::LB)) {
		m_GridMinEdge = m_pGrid->Get_MinEdge();
	}

	if (CGameInstance::GetInstance()->Get_GUISystem()->UsingUI()) return;

	if (m_pInputService->Mouse_Away(MOUSE_BTN::LB)) {
		m_GridMaxEdge = m_pGrid->Get_MaxEdge();

		auto indexArea = m_pTileSystem->Get_IndeciesByArea(
			{ m_GridMinEdge.x,m_GridMinEdge.y,m_GridMinEdge.z,1 },
			{ m_GridMaxEdge.x,m_GridMaxEdge.y,m_GridMaxEdge.z,1 }
		);

		for (TILE_INDEX index : indexArea) {
			//m_pTileSystem.
			DebugIndex tile{ index,m_pTileSystem->Get_InstanceInfoByIndex(index).vMaterialType };
			m_selectedIndex.push_back(tile);
			m_pTileSystem->Set_Material_ID(index, { 0,-1,0,0 });
		}

	}

	if (CGameInstance::GetInstance()->Get_GUISystem()->UsingUI()) return;
}

void CEditorSystem::Clear_Index()
{
	for (auto info : m_selectedIndex) {
		m_pTileSystem->Set_Material_ID(info.index, info.saveMtl);
	}
	m_selectedIndex.clear();
}

void CEditorSystem::Adjust_Height()
{
	for (auto index : m_selectedIndex) {
		_float4 cornerHeight = m_EditorContext.m_fConerHeight;
		m_pTileSystem->Change_CornerHeight(index.index, cornerHeight.x, cornerHeight.y, cornerHeight.z, cornerHeight.w);
	}
}

void CEditorSystem::Adjust_Flag()
{
	for (auto index : m_selectedIndex) {
		_float4 cornerHeight = m_EditorContext.m_fConerHeight;
		m_pTileSystem->Add_TileFlagByIndex(index.index, m_EditorContext.tileFlag);
	}
}


void CEditorSystem::Adjust_Material()
{
	for (auto& index : m_selectedIndex) {
		_float4 cornerHeight = m_EditorContext.m_fConerHeight;
		m_pTileSystem->Set_Material_ID(index.index, {0,0,0,0});
	}
}

HRESULT CEditorSystem::Delete_Object(CGameObject* pObject)
{
	if (pObject)
		m_pObjMgr->Remove_Object(pObject);

	return S_OK;
}


HRESULT CEditorSystem::Load_MapData()
{
	string Systempath = "../../Resources/Data/TileSystemData.dat";

	HRESULT TileInfo = CGameInstance::GetInstance()->Excute_TileSystemByData(Systempath);
	if (FAILED(TileInfo))
	{
		TILESYSTEM_INFO info = {};
		info.iTileCountX = 16 * 6 * 2;
		info.iTileCountZ = 16 * 6 * 2;
		info.vWorldMin = { 0,0,0 };
		info.vWorldMax = { info.iTileCountX * 10.f,	15.f	, info.iTileCountZ * 10.f };

		CGameInstance::GetInstance()->Excute_TileSystem(info);
	}

	m_pTileSystem = CGameInstance::GetInstance()->Get_TileSystem();
 	m_pTileSystem->Execute_InstanceModel(G_GlobalLevelKey, "Base_0.model", "Base_0.mat");
	m_EditorContext.ContextTileInfo = m_pTileSystem->Get_TileSystemInfo();
	if (FAILED(TileInfo))
	{
		return S_OK;
	}
	string path = "../../Resources/Data/MapData.dat";
	ifstream ifs(path.c_str(), ios::binary);

	if (!ifs.is_open())
	{
		return E_FAIL;
	}

	MAP_FILE_HEADER MapFile = {};
	ifs.read(reinterpret_cast<char*>(&MapFile), sizeof(MAP_FILE_HEADER));
	for (size_t i = 0; i < MapFile.iObjectCount; i++)
	{
		MAP_OBJECT_HEADER ObjectHeader = {};
		ifs.read(reinterpret_cast<char*>(&ObjectHeader), sizeof(MAP_OBJECT_HEADER));

		CGameObject* pObject =
			Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_MapObject" })
			.Position({ 0,0,0 })
			.Scale({ 1,1,1 })
			.Build("Obj");

		if (pObject) {
			m_pObjMgr->Add_Object(pObject, { "Editor_Level","MapObject_Layer" });
			dynamic_cast<CMapObject*>(pObject)->Load_Object(ObjectHeader);
		}
		else
			Safe_Release(pObject);
	}
	for (size_t i = 0; i < MapFile.iTileCount; i++)
	{
		NEW_MAP_TILE_HEADER Tile_Header = {};
		ifs.read(reinterpret_cast<char*>(&Tile_Header), sizeof(NEW_MAP_TILE_HEADER));

		CTileObject::TILE_TYPE_DESC* objDesc = new CTileObject::TILE_TYPE_DESC;
		objDesc->TypeName = string(Tile_Header.BaseTypeName);
		objDesc->index = Tile_Header.Index;

		CGameObject* pObject =
			Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Tile" })
			.Add_ObjDesc(objDesc)
			.Position({ 0,Tile_Header.height<0.2? 0.2f: Tile_Header.height,0 })
			.Scale({ 1,1,1 })
			.Build(objDesc->TypeName);

		if (pObject)
			m_pObjMgr->Add_Object(pObject, { "Editor_Level","Tile_Layer" });
		else
			Safe_Release(pObject);
	}
	ifs.close();
	return S_OK;
}

HRESULT CEditorSystem::Save_MapData()
{
	string path = "../../Resources/Data/MapData.dat";

	ofstream ofs(path.c_str(), ios::binary);
	if (!ofs.is_open())
		return E_FAIL;

	/*타일 전체 정보 저장*/
	MAP_FILE_HEADER MapFile = {};

	MapFile.iObjectCount = 0;
	MapFile.iTileCount = 0;

	auto MapObjectLayer = m_pObjMgr->Get_Layer({ "Editor_Level","MapObject_Layer" });
	if (MapObjectLayer) {
		MapFile.iObjectCount = MapObjectLayer->Get_ObjectCount();
	}

	auto AutoTileLayer = m_pObjMgr->Get_Layer({ "Editor_Level","Tile_Layer" });
	if (AutoTileLayer) {
		MapFile.iTileCount = AutoTileLayer->Get_ObjectCount();
	}

	ofs.write(reinterpret_cast<const char*>(&MapFile), sizeof(MAP_FILE_HEADER));

	if (MapObjectLayer) {
		auto objectVector = MapObjectLayer->Get_AllObject();
		if (!objectVector.empty())
			for (auto& mapObject : objectVector) {
				if(mapObject)
					dynamic_cast<CMapObject*>(mapObject)->Save_MapData(ofs);
			}
	}

	if (AutoTileLayer) {
		auto TileVector = AutoTileLayer->Get_AllObject();
		if (!TileVector.empty())
			for (auto& TileObject : TileVector) {
				dynamic_cast<CTileObject*>(TileObject)->Save_MapData(ofs);
			}
	}

	ofs.close();

	return S_OK;
}

void CEditorSystem::Free()
{
	__super::Free();
	Safe_Release(m_pGrid);
	Safe_Release(m_pDirectoryPanel);
}
