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
#include "Field_Tree.h"
#include "Field_Stone.h"

#include "Plant_Tree.h"
#include "AutoTile.h"

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
	CGameInstance::GetInstance()->Get_TileSystem()->Execute_InstanceModel(G_GlobalLevelKey, "Base_0.model", "Base_0.mat");

	auto pProto = CGameInstance::GetInstance()->Get_PrototypeMgr();
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	auto pObjMgr = CGameInstance::GetInstance()->Get_ObjectMgr();

	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_FieldOut", CField_Out::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_FieldTree", CField_Tree::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_FieldStone", CField_Stone::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_PlantTree", CPlant_Tree::Create());

	for (size_t i = 0; i < mapFileHeader.iObjectCount; i++)
	{
		MAP_OBJECT_HEADER objHeader = {};
		ifs.read(reinterpret_cast<char*>(&objHeader), sizeof(MAP_OBJECT_HEADER));
		auto iter = g_ModelMapTable.find(objHeader.Object_type);
		if (iter == g_ModelMapTable.end())
			continue;
		CGameObject* pObject = { nullptr };
		if (objHeader.Object_type < 55) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldOut" })
				.Build(iter->second[0]);
		}
		else  if (objHeader.Object_type > 55 && objHeader.Object_type< 66) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldTree" })
				.Build(iter->second[0]);
		}
		else  if (objHeader.Object_type > 66 && objHeader.Object_type < 72) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldStone" })
				.Build(iter->second[0]);
		}
		else  if (objHeader.Object_type > 72 && objHeader.Object_type < 85) {
			pObject =
				Builder::Create_Object({ Desc.LevelTag , "GameObject_PlantTree" })
				.Build(iter->second[0]);
		}
		else {
			continue;
		}

		if (pObject) {
			dynamic_cast<CFieldObject*>(pObject)->Sync_MapData(objHeader, iter->second);
			pObjMgr->Add_Object(pObject, Desc);
		}
	}

	for (size_t i = 0; i < mapFileHeader.iTileCount; i++)
	{
		MAP_TILE_HEADER Tile_Header = {};
		ifs.read(reinterpret_cast<char*>(&Tile_Header), sizeof(MAP_TILE_HEADER));

		CAutoTile::TILE_TYPE_DESC* objDesc = new CAutoTile::TILE_TYPE_DESC;
		objDesc->TypeName = string(Tile_Header.BaseTypeName);
		objDesc->index = Tile_Header.Index;

		CGameObject* pObject =
			Builder::Create_Object({ "GamePlay_Level", "GamePlay_GameObject_AutoTile"})
			.Position({ 0,0,0 })
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

			if (key.empty())
				continue;

			g_ModelMapTable[id] = { key, model, mat, modelPath, matPath };
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
