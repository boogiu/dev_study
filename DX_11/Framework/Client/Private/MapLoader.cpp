#include "Client_Defines.h"
#include "MapLoader.h"
#include "GameInstance.h"

#include "IProtoService.h"
#include "IObjectService.h"
#include "ITileService.h"
#include "IResourceService.h"
#include "FieldStructure.h"
#include "FieldOut.h"
#include "BaseField.h"
#include "Builder.h"
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
	
	CGameInstance::GetInstance()->Excute_TileSystem(mapFileHeader.tileInfo);
	auto pProto = CGameInstance::GetInstance()->Get_PrototypeMgr();
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	auto pObjMgr = CGameInstance::GetInstance()->Get_ObjectMgr();

	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_BaseField", CBaseField::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_FieldOut", CFieldOut::Create());
	pProto->Add_ProtoType(Desc.LevelTag, "GameObject_FieldStructure", CFieldStructure::Create());

	for (size_t i = 0; i < mapFileHeader.iFieldOutCount; i++)
	{
		MAP_OBJECT_HEADER objHeader = {};
		ifs.read(reinterpret_cast<char*>(&objHeader), sizeof(MAP_OBJECT_HEADER));

		pRcsMgr->Add_ResourcePath(objHeader.ModelName, objHeader.ModelPath);
		pRcsMgr->Add_ResourcePath(objHeader.MaterialName, objHeader.MaterialPath);

		CFieldOut::FIELDOUT_DESC* ObjDesc = new CFieldOut::FIELDOUT_DESC;
		ObjDesc->ModelName = objHeader.ModelName;
		ObjDesc->MaterialName = objHeader.MaterialName;
		ObjDesc->Index = objHeader.Index;
		ObjDesc->LevelTag = Desc.LevelTag;

		CGameObject* pFieldOut =
			Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldOut" })
			.Position({ objHeader.vWorldPos.x,objHeader.vWorldPos.y,objHeader.vWorldPos.z })
			.Add_ObjDesc(ObjDesc)
			.Build(objHeader.ModelName);

		pObjMgr->Add_Object(pFieldOut, Desc);
	}

	for (size_t i = 0; i < mapFileHeader.iStructureCount; i++)
	{
		MAP_OBJECT_HEADER objHeader = {};
		ifs.read(reinterpret_cast<char*>(&objHeader), sizeof(MAP_OBJECT_HEADER));

		pRcsMgr->Add_ResourcePath(objHeader.ModelName, objHeader.ModelPath);
		pRcsMgr->Add_ResourcePath(objHeader.MaterialName, objHeader.MaterialPath);

		CFieldStructure::FIELDSTR_DESC* ObjDesc = new CFieldStructure::FIELDSTR_DESC;
		ObjDesc->ModelName = objHeader.ModelName;
		ObjDesc->MaterialName = objHeader.MaterialName;
		ObjDesc->Index = objHeader.Index;
		ObjDesc->LevelTag = Desc.LevelTag;

		CGameObject* pFieldOut =
			Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldStructure" })
			.Position({ objHeader.vWorldPos.x,objHeader.vWorldPos.y,objHeader.vWorldPos.z })
			.Add_ObjDesc(ObjDesc)
			.Build(objHeader.ModelName);

		pObjMgr->Add_Object(pFieldOut, Desc);
	}

	CGameObject* pBaseField =
		Builder::Create_Object({ Desc.LevelTag, "GameObject_BaseField" })
		.Position({ 0,-0.05f,0 })
		.Build("Base_Plane");

	pObjMgr->Add_Object(pBaseField, { Desc.LevelTag,"Base_Field" });

	CBaseField* pBaseFieldCast = dynamic_cast<CBaseField*>(pBaseField);
	pBaseFieldCast->Load_BaseTile(ifs, mapFileHeader.iBaseFieldCount);

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

	for (size_t i = 0; i < mapFileHeader.iMapObjectCount; i++)
	{
		MAP_OBJECT_HEADER objHeader = {};
		ifs.read(reinterpret_cast<char*>(&objHeader), sizeof(MAP_OBJECT_HEADER));

		pRcsMgr->Add_ResourcePath(objHeader.ModelName, objHeader.ModelPath);
		pRcsMgr->Add_ResourcePath(objHeader.MaterialName, objHeader.MaterialPath);

		CFieldStructure::FIELDSTR_DESC* ObjDesc = new CFieldStructure::FIELDSTR_DESC;
		ObjDesc->ModelName = objHeader.ModelName;
		ObjDesc->MaterialName = objHeader.MaterialName;
		ObjDesc->Index = objHeader.Index;
		ObjDesc->LevelTag = Desc.LevelTag;

		CGameObject* pFieldOut =
			Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldStructure" })
			.Position({ objHeader.vWorldPos.x,objHeader.vWorldPos.y,objHeader.vWorldPos.z })
			.Add_ObjDesc(ObjDesc)
			.Build(objHeader.ModelName);

		pObjMgr->Add_Object(pFieldOut, Desc);
	}
	ifs.close();
	return S_OK;
}
