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

	{

		MAP_BASE_HEADER BaseHeader = {};
		ifs.read(reinterpret_cast<char*>(&BaseHeader), sizeof(MAP_BASE_HEADER));

		pRcsMgr->Add_ResourcePath(BaseHeader.ModelName, BaseHeader.ModelPath);
		pRcsMgr->Add_ResourcePath(BaseHeader.MaterialName, BaseHeader.MaterialPath);

		_float4 pos = { BaseHeader.vWorldPos };

		CBaseField::BASEFIELD_DESC* ObjDesc = new CBaseField::BASEFIELD_DESC;
		ObjDesc->ModelName = BaseHeader.ModelName;
		ObjDesc->MaterialName = BaseHeader.MaterialName;
		ObjDesc->LevelTag = Desc.LevelTag;

		CGameObject* pObject =
			Builder::Create_Object({ Desc.LevelTag, "GameObject_BaseField" })
			.Position({ pos.x,mapFileHeader.tileInfo.vWorldMin.y,pos.z })
			//.Scale(BaseHeader.vWorldScale)
			.Add_ObjDesc(ObjDesc)
			.Build("Base_Plane");

		pObjMgr->Add_Object(pObject, { Desc.LevelTag,"Base_Plane" });
	}

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
	}
	for (size_t i = 0; i < mapFileHeader.iTileCount; i++)
	{
		MAP_TILE_HEADER mapTileHeader = {};
		ifs.read(reinterpret_cast<char*>(&mapTileHeader), sizeof(MAP_TILE_HEADER));
	}
	ifs.close();
	return S_OK;
}
