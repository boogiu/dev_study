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
#include "StructureObject.h"

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

	/* 오브젝트*/
	IProtoService* pProto = CGameInstance::GetInstance()->Get_PrototypeMgr();
	pProto->Add_ProtoType(G_GlobalLevelKey, "Proto_GameObject_Tile", CTileObject::Create());
	pProto->Add_ProtoType(G_GlobalLevelKey, "Proto_GameObject_FieldOut", CFieldOutBlocks::Create());
	pProto->Add_ProtoType(G_GlobalLevelKey, "Proto_GameObject_Structure", CStructureObject::Create());

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
	DragDrop_Object();

	m_pRayManager->Register_Ray(&m_tRay);
}


void CEditorSystem::Execute_TileSystem()
{
	HRESULT hasFile = Load_MapData();
	TILESYSTEM_INFO info = m_EditorContext.ContextTileInfo;

	if (FAILED(hasFile)) {
		info.iTileCountX = 16 * 6 * 2;
		info.iTileCountY = 4;/*0~4 -> 5칸*/
		info.iTileCountZ = 16 * 6 * 2;
		info.vWorldMin = { 0,0,0 };
		info.vWorldMax = { info.iTileCountX * 10.f,	info.iTileCountY * 15.f	, info.iTileCountZ * 10.f };
	}
	
	/*타일 시스템 구동*/
	CGameInstance::GetInstance()->Excute_TileSystem(info);
	m_pTileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	m_EditorContext.ContextTileInfo = info;

	/*그리드 역할의 오브젝트 생성*/
	IProtoService* pProto = CGameInstance::GetInstance()->Get_PrototypeMgr();
	pProto->Add_ProtoType(G_GlobalLevelKey, "Proto_GameObject_Grid", CGridObject::Create());

	CGameObject* pGrid = Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Grid" })
		.Position(info.HalfPoint())
		.Scale(info.WorldSize())
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

	/*베이스 타일을 생성*/
	CGameObject* pObject =
		Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Tile" })
		.Position({ info.HalfPoint().x , info.vWorldMin.y - 0.1f ,info.HalfPoint().z })
		.Scale({ 16 * 8 + 2 + 2 , 0 , 16 * 6 + 4 + 4 })
		.Build("Base_Plane", &m_BaseTileID);

	m_pObjMgr->Add_Object(pObject, { "Editor_Level","Base_Layer" });
	dynamic_cast<CTileObject*>(pObject)->Link_Data("Base_0", true);

	return;
}

void CEditorSystem::DragDrop_Object()
{
	if (nullptr == m_EditorContext.pEditingObject) return;
	CGameObject* obj = m_EditorContext.pEditingObject;

	TILE_INDEX index = m_pGrid->Get_HitIndex();

	if (m_pInputService->Mouse_Hold(MOUSE_BTN::LB)) {
		switch (m_EditorContext.eType)
		{
		case TILE: {
			obj->Get_Component<CTransform>()->Set_Pos({ m_pGrid->Get_HitPos() });
		}
		break;
		case STRUCTURE: 
		case FIELDOUT: {
			_float4 DragPos = m_pTileSystem->Get_PositionByIndex(index, ANCHOR::Right | ANCHOR::Bottom);
			obj->Get_Component<CTransform>()->Set_Pos({ DragPos.x,DragPos.y,DragPos.z });
		}
		break;
		default:
			break;
		}
	}

	if (m_pInputService->Mouse_Away(MOUSE_BTN::LB)) {
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
		case STRUCTURE: {
			CStructureObject* object = dynamic_cast<CStructureObject*>(obj);
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

HRESULT CEditorSystem::Load_MapData()
{
	string path = "../../Resources/Data/MapData.dat";
	filesystem::path directory(path);

	ifstream ifs(path.c_str(), ios::binary);
	if(!ifs.is_open())
		return E_FAIL;

	/*타일 전체 정보 저장*/
	MAP_FILE_HEADER MapFile = {};
	ifs.read(reinterpret_cast<char*>(&MapFile), sizeof(MAP_FILE_HEADER));

	m_EditorContext.ContextTileInfo = MapFile.tileInfo;

	for (size_t i = 0; i < MapFile.iFieldOutCount; i++)
	{
		MAP_OBJECT_HEADER ObjHeader = {};
		ifs.read(reinterpret_cast<char*>(&ObjHeader), sizeof(MAP_OBJECT_HEADER));

		CGameObject* pObject =
			Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_FieldOut" })
			.Position({ 0,0,0 })
			.Scale({ 1,1,1 })
			.Build(ObjHeader.ModelName);

		HRESULT hr = dynamic_cast<CFieldOutBlocks*>(pObject)->Load_Object(ObjHeader);

		if (SUCCEEDED(hr))
			m_pObjMgr->Add_Object(pObject, { "Editor_Level","FieldOut_Layer" });
		else
			Safe_Release(pObject);
	}

	for (size_t i = 0; i < MapFile.iStructureCount; i++)
	{
		MAP_OBJECT_HEADER ObjHeader = {};
		ifs.read(reinterpret_cast<char*>(&ObjHeader), sizeof(MAP_OBJECT_HEADER));

		CGameObject* pObject =
			Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Structure" })
			.Position({ 0,0,0 })
			.Scale({ 1,1,1 })
			.Build(ObjHeader.ModelName);

		HRESULT hr = dynamic_cast<CStructureObject*>(pObject)->Load_Object(ObjHeader);

		if (SUCCEEDED(hr))
			m_pObjMgr->Add_Object(pObject, { "Editor_Level","Structure_Layer" });
		else
			Safe_Release(pObject);
	}


	ifs.close();
	return S_OK;
}

HRESULT CEditorSystem::Save_MapData()
{
	string path = "../../Resources/Data/MapData.dat";
	filesystem::path directory(path);

	ofstream ofs(path.c_str(), ios::binary);
	if (!ofs.is_open())
		return E_FAIL;

	/*타일 전체 정보 저장*/
	MAP_FILE_HEADER MapFile = {};
	MapFile.tileInfo = m_EditorContext.ContextTileInfo;

	MapFile.iFieldOutCount = 0;
	MapFile.iStructureCount = 0;
	MapFile.iTileCount = 0;

	/*1. FieldOut Tile*/
	auto FieldOutLayer = m_pObjMgr->Get_Layer({ "Editor_Level","FieldOut_Layer" });

	if (FieldOutLayer)
		MapFile.iFieldOutCount = FieldOutLayer->Get_ObjectCount();

	/*2. Structure Tile*/
	auto StructureLayer = m_pObjMgr->Get_Layer({ "Editor_Level","Structure_Layer" });
	if (StructureLayer)
		MapFile.iStructureCount = StructureLayer->Get_ObjectCount();

	///*3. Tile Tile*/
	//auto TileLayer = m_pObjMgr->Get_Layer({ "Editor_Level","Tile_Layer" });
	//if (TileLayer)
	//	MapFile.iTileCount = TileLayer->Get_ObjectCount();

	ofs.write(reinterpret_cast<char*>(&MapFile), sizeof(MAP_FILE_HEADER));

	{
		auto& ObjectVector = FieldOutLayer->Get_AllObject();
		for (auto& FieldOut : ObjectVector) {
			CFieldOutBlocks* pFiedlOut = dynamic_cast<CFieldOutBlocks*>(FieldOut);
			if(pFiedlOut)
				pFiedlOut->Save_MapData(ofs);
		}
	}

	{
		auto& ObjectVector = StructureLayer->Get_AllObject();
		for (auto& structure : ObjectVector) {
			CStructureObject* pStructure = dynamic_cast<CStructureObject*>(structure);
			if (pStructure)
				pStructure->Save_MapData(ofs);
		}
	}

	//
	//	auto& ObjectVector = TileLayer->Get_AllObject();
	//	for (auto& Tile : ObjectVector) {
	//		CTileObject* Tile = dynamic_cast<CTileObject*>(Tile);
	//		if (Tile)
	//			Tile->Save_MapData(ofs);
	//	}
	//

	ofs.close();

	return S_OK;
}

HRESULT CEditorSystem::Create_MapObject(const string& folderName, ObjType eType)
{
	if (m_EditorContext.pEditingObject != nullptr) return E_FAIL;
	CGameObject* pObject = { nullptr };

	switch (eType)
	{
	case MapEditor::CEditorSystem::TILE: {
		pObject = Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Tile" })
			.Position({ 0,0,0 })
			.Scale({ 1,1,1 })
			.Build(folderName);
		m_pObjMgr->Add_Object(pObject, { "Editor_Level","Tile_Layer" });
		dynamic_cast<CTileObject*>(pObject)->Link_Data(folderName);
	}
		break;
	case MapEditor::CEditorSystem::FIELDOUT: {
		pObject = Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_FieldOut" })
			.Position({ 0,0,0 })
			.Scale({ 1,1,1 })
			.Build(folderName);
		m_pObjMgr->Add_Object(pObject, { "Editor_Level","FieldOut_Layer" });
		dynamic_cast<CFieldOutBlocks*>(pObject)->Link_Data(folderName);
	}
		break;
	case MapEditor::CEditorSystem::STRUCTURE: {
		pObject = Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Structure" })
			.Position({ 0,0,0 })
			.Scale({ 1,1,1 })
			.Build(folderName);
		m_pObjMgr->Add_Object(pObject, { "Editor_Level","Structure_Layer" });
		dynamic_cast<CStructureObject*>(pObject)->Link_Data(folderName);
	}
		break;
	default:
		break;
	}

	m_EditorContext.pEditingObject = pObject;
	m_EditorContext.eType = eType;
	return S_OK;

}

HRESULT CEditorSystem::Delete_Object(CGameObject* pObject)
{
	if (pObject)
		m_pObjMgr->Remove_Object(pObject);

	return S_OK;
}

void CEditorSystem::Free()
{
	__super::Free();
	Safe_Release(m_pGrid);
	Safe_Release(m_pDirectoryPanel);
}
