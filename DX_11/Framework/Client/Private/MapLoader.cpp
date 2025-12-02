#include "Client_Defines.h"
#include "MapLoader.h"
#include "GameInstance.h"

#include "IProtoService.h"
#include "IObjectService.h"
#include "ITileService.h"
#include "IResourceService.h"
#include "Builder.h"

#include "FieldObject.h"
#include "Field_Out.h"
#include "Field_Stone.h"
#include "Field_Bridge.h"
#include "Field_Structure.h"
#include "Plant_Tree.h"
#include "AutoTile.h"
#include "Plant_Grass.h"
#include "Furniture.h"

//0~60 필드 아웃
//60~70 나무
//70~80 잡초
//80~90 돌
//90~100 다리
//101~200 구조물
//201~인도어

_bool isFieldOut(_uint type) { return 0 <= type && type < 100; }
_bool isTree(_uint type) { return 100 <= type && type < 200; }
_bool isWeed(_uint type) { return 200 <= type && type < 300; }
_bool isFlower(_uint type) { return 300 <= type && type < 400; }
_bool isStone(_uint type) { return 400 <= type && type < 500; }
_bool isStructure(_uint type) { return 500 <= type && type < 600; }
_bool isFurniture(_uint type) { return 600 <= type && type < 700;}
_bool isBridge(_uint type) { return 700 <= type && type < 800; }

CMapLoader::CMapLoader()
{
}

HRESULT CMapLoader::Load_MapData(string filePath, const LAYER_DESC& Desc)
{
	ifstream ifs(filePath.c_str(), ios::binary);

	if (!ifs.is_open())
		return E_FAIL;

	MAP_FILE_HEADER mapFileHeader = {};
	ifs.read(reinterpret_cast<char*>(&mapFileHeader), sizeof(MAP_FILE_HEADER));
	string Systempath = "../../Resources/Data/TileSystemData.dat";
	CGameInstance::GetInstance()->Excute_TileSystemByData(Systempath);
	CGameInstance::GetInstance()->Get_TileSystem()->Execute_InstanceModel(G_GlobalLevelKey, "Base_0.model", "Base_0.mat","Instancing");

	auto pProto = CGameInstance::GetInstance()->Get_PrototypeMgr();
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	auto pObjMgr = CGameInstance::GetInstance()->Get_ObjectMgr();

	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_FieldOut", CField_Out::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_FieldStone", CField_Stone::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_PlantTree", CPlant_Tree::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_FieldBridge", CField_Bridge::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_FieldStructure", CField_Structure::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_PlantGrass", CPlant_Grass::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_Furniture", CFurniture::Create());

	for (size_t i = 0; i < mapFileHeader.iObjectCount; i++)
	{
		NEW_MAP_OBJECT_HEADER objHeader = {};
		ifs.read(reinterpret_cast<char*>(&objHeader), sizeof(NEW_MAP_OBJECT_HEADER));
		auto iter = g_ModelMapTable.find(objHeader.Object_type);
		if (iter == g_ModelMapTable.end())
			continue;
		CGameObject* pObject = { nullptr };
		if (isFieldOut(objHeader.Object_type)) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldOut" })
				.Build(iter->second[0]);
		}
		else if (isTree(objHeader.Object_type)){
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_PlantTree" })
				.Build(iter->second[0]);
		}
		else if (isStone(objHeader.Object_type)) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldStone" })
				.Build(iter->second[0]);
		}
		else if (isBridge(objHeader.Object_type)) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldBridge" })
				.Build(iter->second[0]);
		}
		else if (isStructure(objHeader.Object_type)) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldStructure" })
				.Build(iter->second[0]);
		}
		else if (isWeed(objHeader.Object_type)) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_PlantGrass" })
				.Build(iter->second[0]);
		}
		else if (isFurniture(objHeader.Object_type)) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_Furniture" })
				.Build(iter->second[0]);

			dynamic_cast<CFurniture*>(pObject)->Make_FurnitureByMapData(objHeader, iter->second);
		}
		else {
			continue;
		}

		if (pObject) {
			auto Obj = dynamic_cast<CFieldObject*>(pObject);
			if(Obj)
				Obj->Sync_MapData(objHeader, iter->second);

			pObjMgr->Add_Object(pObject, Desc);
		}
	}

	for (size_t i = 0; i < mapFileHeader.iTileCount; i++)
	{
		NEW_MAP_TILE_HEADER Tile_Header = {};
		ifs.read(reinterpret_cast<char*>(&Tile_Header), sizeof(NEW_MAP_TILE_HEADER));

		CAutoTile::TILE_TYPE_DESC* objDesc = new CAutoTile::TILE_TYPE_DESC;
		objDesc->TypeName = string(Tile_Header.BaseTypeName);
		objDesc->index = Tile_Header.Index;

		CGameObject* pObject =
			Builder::Create_Object({ "GamePlay_Level", "GamePlay_GameObject_AutoTile"})
			.Position({ 0,Tile_Header.height,0 })
			.Scale({ 1,1,1 })
			.Add_ObjDesc(objDesc)
			.Build(objDesc->TypeName);

		if (pObject)
			pObjMgr->Add_Object(pObject, { Desc.LevelTag,"Tile_Layer" });
		else
			Safe_Release(pObject);
	}

	ifs.close();
	return S_OK;
}

HRESULT CMapLoader::Load_ModelData()
{
	wstring path = L"../../Resources/Data/ModelMap.json";
	ifstream ifs(path);
	if (!ifs.is_open()) {
		MessageBoxW(nullptr, L"ModelMap.json 파일을 찾을 수 없습니다.", L"Error", MB_OK);
		return E_FAIL;
	}

	json jScene;
	try {
		ifs >> jScene;
	}
	catch (const json::parse_error& e) {
		MessageBoxA(nullptr, e.what(), "JSON Parse Error", MB_OK);
		return E_FAIL;
	}
	ifs.close();

	g_ModelMapTable.clear();

	// JSON 배열 순회
	for (auto& item : jScene)
	{
		try
		{
			string key = item.value("Key", "");
			_uint id = 0;
			if (item.contains("ID"))
			{
				if (item["ID"].is_number_integer())
					id = item["ID"].get<_uint>();
				else if (item["ID"].is_string())
					id = std::stoul(item["ID"].get<string>());
			}
			string model = item.value("Model", "");
			string mat = item.value("Mat", "");
			string modelPath = item.value("ModelPath", "");
			string matPath = item.value("MatPath", "");
			string AdditionalData = item.value("AdditionalData", "");

			if (key.empty())
				continue;

			g_ModelMapTable[id] = { key, model, mat, modelPath, matPath ,AdditionalData };
		}
		catch (...)
		{
			continue;
		}
	}

	// 리소스 매니저에 등록
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	for (auto& pair : g_ModelMapTable)
	{
		const vector<string>& values = pair.second;
		if (values.size() < 5)
			continue;

		pRcsMgr->Add_ResourcePath(values[1], values[3]); // model
		pRcsMgr->Add_ResourcePath(values[2], values[4]); // mat
	}

	return S_OK;
}

HRESULT CMapLoader::Load_Sequential(const LAYER_DESC& Desc)
{
	if (reservedMapObj.empty() && reservedMapTile.empty())
		return S_OK;
	auto pObjMgr = CGameInstance::GetInstance()->Get_ObjectMgr();

	if (!reservedMapTile.empty()) {
		NEW_MAP_TILE_HEADER Tile_Header = reservedMapTile.front();
		reservedMapTile.pop();

		CAutoTile::TILE_TYPE_DESC* objDesc = new CAutoTile::TILE_TYPE_DESC;
		objDesc->TypeName = string(Tile_Header.BaseTypeName);
		objDesc->index = Tile_Header.Index;
		CGameObject* pObject = {nullptr};
		 pObject =
			Builder::Create_Object({ "GamePlay_Level", "GamePlay_GameObject_AutoTile" })
			.Position({ 0,Tile_Header.height,0 })
			.Scale({ 1,1,1 })
			.Add_ObjDesc(objDesc)
			.Build(objDesc->TypeName);

		if (pObject)
			pObjMgr->Add_Object(pObject, { Desc.LevelTag,"Tile_Layer" });
		else
			Safe_Release(pObject);

		return S_OK;
	}

	if (!reservedMapObj.empty()) {
		NEW_MAP_OBJECT_HEADER header = reservedMapObj.front();
		reservedMapObj.pop();
		auto iter = g_ModelMapTable.find(header.Object_type);
		if (iter == g_ModelMapTable.end())
			return E_FAIL;
		CGameObject* pObject = { nullptr };
		if (isFieldOut(header.Object_type)) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldOut" })
				.Build(iter->second[0]);
		}
		else if (isTree(header.Object_type)) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_PlantTree" })
				.Build(iter->second[0]);
		}
		else if (isStone(header.Object_type)) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldStone" })
				.Build(iter->second[0]);
		}
		else if (isBridge(header.Object_type)) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldBridge" })
				.Build(iter->second[0]);
		}
		else if (isStructure(header.Object_type)) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldStructure" })
				.Build(iter->second[0]);
		}
		else if (isWeed(header.Object_type)||isFlower(header.Object_type)) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_PlantGrass" })
				.Build(iter->second[0]);
		}
		else if (isFurniture(header.Object_type)) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_Furniture" })
				.Build(iter->second[0]);

			dynamic_cast<CFurniture*>(pObject)->Make_FurnitureByMapData(header, iter->second);
		}
		else {
			return E_FAIL;
		}

		if (pObject) {
			auto Obj = dynamic_cast<CFieldObject*>(pObject);
			if (Obj)
				Obj->Sync_MapData(header, iter->second);

			pObjMgr->Add_Object(pObject, Desc);

			return S_OK;
		}
	}

	return S_OK;
}
CMapLoader* CMapLoader::Create()
{
	return new CMapLoader;
}

void CMapLoader::Free()
{
	__super::Free();
}

HRESULT CMapLoader::Reserved_Load(string filePath, const LAYER_DESC& Desc)
{
	ifstream ifs(filePath.c_str(), ios::binary);

	if (!ifs.is_open())
		return E_FAIL;

	MAP_FILE_HEADER mapFileHeader = {};
	ifs.read(reinterpret_cast<char*>(&mapFileHeader), sizeof(MAP_FILE_HEADER));
	string Systempath = "../../Resources/Data/TileSystemData.dat";
	CGameInstance::GetInstance()->Excute_TileSystemByData(Systempath);
	CGameInstance::GetInstance()->Get_TileSystem()->Execute_InstanceModel(G_GlobalLevelKey, "Base_0.model", "Base_0.mat", "Instancing");

	auto pProto = CGameInstance::GetInstance()->Get_PrototypeMgr();
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	auto pObjMgr = CGameInstance::GetInstance()->Get_ObjectMgr();

	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_FieldOut", CField_Out::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_FieldStone", CField_Stone::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_PlantTree", CPlant_Tree::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_FieldBridge", CField_Bridge::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_FieldStructure", CField_Structure::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_PlantGrass", CPlant_Grass::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_Furniture", CFurniture::Create());

	for (size_t i = 0; i < mapFileHeader.iObjectCount; i++)
	{
		NEW_MAP_OBJECT_HEADER objHeader = {};
		ifs.read(reinterpret_cast<char*>(&objHeader), sizeof(NEW_MAP_OBJECT_HEADER));
		reservedMapObj.push(objHeader);
	}

	for (size_t i = 0; i < mapFileHeader.iTileCount; i++)
	{
		NEW_MAP_TILE_HEADER Tile_Header = {};
		ifs.read(reinterpret_cast<char*>(&Tile_Header), sizeof(NEW_MAP_TILE_HEADER));
		reservedMapTile.push(Tile_Header);
	}

	ifs.close();
	return S_OK;
}
