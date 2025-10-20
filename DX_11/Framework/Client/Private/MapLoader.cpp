#include "Client_Defines.h"
#include "MapLoader.h"
#include "GameInstance.h"

#include "IProtoService.h"
#include "IObjectService.h"
#include "ITileService.h"
#include "IResourceService.h"

#include "FieldOut.h"
#include "BaseField.h"
#include "Builder.h"

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

	for (size_t i = 0; i < mapFileHeader.iFieldOutCount; i++)
	{
		MAP_OBJECT_HEADER objHeader = {};
		ifs.read(reinterpret_cast<char*>(&objHeader), sizeof(MAP_OBJECT_HEADER));

	//pRcsMgr->Add_ResourcePath(objHeader.ModelName, objHeader.ModelPath);
	//pRcsMgr->Add_ResourcePath(objHeader.MaterialName, objHeader.MaterialPath);
	//
	//CFieldOut::FIELDOUT_DESC* ObjDesc = new CFieldOut::FIELDOUT_DESC;
	//ObjDesc->ModelName = objHeader.ModelName;
	//ObjDesc->MaterialName = objHeader.MaterialName;
	//ObjDesc->Index = objHeader.Index;
	//ObjDesc->LevelTag = Desc.LevelTag;
	//
	//CGameObject* pFieldOut =
	//	Builder::Create_Object({ Desc.LevelTag , "GameObject_FieldOut" })
	//	.Position({ objHeader.vWorldPos.x,objHeader.vWorldPos.y,objHeader.vWorldPos.z })
	//	.Add_ObjDesc(ObjDesc)
	//	.Build(objHeader.ModelName);
	//
	//pObjMgr->Add_Object(pFieldOut, Desc);
	}

	for (size_t i = 0; i < mapFileHeader.iStructureCount; i++)
	{
		MAP_OBJECT_HEADER objHeader = {};
		ifs.read(reinterpret_cast<char*>(&objHeader), sizeof(MAP_OBJECT_HEADER));
	}

	CGameObject* pBaseField =
		Builder::Create_Object({ Desc.LevelTag, "GameObject_BaseField" })
		.Position({ 0,0,0 })
		.Build("Base_Plane");

	pObjMgr->Add_Object(pBaseField, { Desc.LevelTag,"Base_Field" });

	CBaseField* pBaseFieldCast = dynamic_cast<CBaseField*>(pBaseField);
	for (size_t i = 0; i < mapFileHeader.iTileCount; i++)
	{
		INSTANCE_TILE instanceTile = {};
		ifs.read(reinterpret_cast<char*>(&instanceTile), sizeof(INSTANCE_TILE));

		pBaseFieldCast->Load_Tile(instanceTile);
	}
	ifs.close();
	return S_OK;
}
