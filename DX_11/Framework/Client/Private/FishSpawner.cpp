#include "Client_Defines.h"
#include "FishSpawner.h"
#include "EventSystem.h"
#include "GameInstance.h"
#include "ResourceMgr.h"
#include "Level.h"

#include "Fish_Object.h"
#include "Helper_Func.h"
#include "ItemSpawner.h"

CFishSpawner::CFishSpawner()
{
}

CFishSpawner::CFishSpawner(const CFishSpawner& rhs)
    :CLevelObject(rhs)
{
}

HRESULT CFishSpawner::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CFishSpawner::Initialize(INIT_DESC* pArg)
{
    __super::Initialize(pArg);
    return S_OK;
}

void CFishSpawner::Awake()
{
}

HRESULT CFishSpawner::Link_ItemSpawner(CItemSpawner* pItemSpawner)
{
	if (m_pItemSpawner)
		Safe_Release(m_pItemSpawner);

	m_pItemSpawner = pItemSpawner;

	Safe_AddRef(m_pItemSpawner);

	return S_OK;
}

void CFishSpawner::Priority_Update(_float dt)
{
	m_fElapsedTime += dt;
}

void CFishSpawner::Update(_float dt)
{
	
	if (m_fElapsedTime > m_fSpawnTime) {
		Spawn_Fish();
	}
}

void CFishSpawner::Late_Update(_float dt)
{
}

HRESULT CFishSpawner::Read_FishData(string filePath)
{
	ifstream ifs(filePath);
	if (!ifs.is_open()) {
		MessageBoxW(nullptr, L"InsectData 파일을 찾을 수 없습니다.", L"Error", MB_OK);
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


	m_FishDataTable.clear();
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	// JSON 배열 순회
	for (auto& item : jScene)
	{
		try
		{
			string key = item.value("Name", "");
			FISH_DATA_DESC data = {};
			if (item.contains("Index"))
			{
				if (item["Index"].is_number_integer())
					data.FishIndex = item["Index"].get<_int>();
				else if (item["Index"].is_string())
					data.FishIndex = std::stoul(item["Index"].get<string>());
			}

			data.FishFileName = item.value("Name", "");
			data.FishName = Helper::ConvertToWideString(item.value("FishName", ""));
			data.modelName = item.value("Name", "") + ".model";
			data.materialName = item.value("Name", "") + ".mat";
			data.IconName = "MenuLayout_" + item.value("IconName", "");
			data.ItemFile = item.value("ItemFile", "");
			data.sizeTag = item.value("sizeTag", "");
			data.Comment = Helper::ConvertToWideString(item.value("Comment", ""));

			if (key.empty())
				continue;

			string modelPath = item.value("model", "") + "/" + item.value("Name", "");
			string materialPath = item.value("mat", "") + "/" + item.value("Name", "");

			pRcsMgr->Add_ResourcePath(data.modelName, modelPath + ".model");		// model
			pRcsMgr->Add_ResourcePath(data.materialName, materialPath + ".mat"); // material

			string directory = filesystem::path(modelPath).parent_path().string();
			pRcsMgr->Add_ResourcePath("Get.anim_" + data.FishFileName, directory + "/Animation/Get.anim");
			pRcsMgr->Add_ResourcePath("Swim.anim_" + data.FishFileName, directory + "/Animation/Swim.anim");

			m_FishDataTable[data.FishFileName] = data;
			_uint index = m_NameTable.size();
			m_NameTable[index] = data.FishFileName;
		}
		catch (...)
		{
			continue;
		}
	}

	return S_OK;
}

//GamePlay_GameObject_Fish
void CFishSpawner::Notice_River(TILE_INDEX index)
{
	_uint Flag = CGameInstance::GetInstance()->Get_TileSystem()->Get_InfoByIndex(index).TileFlag;

	if ((Flag & TILE_FLAG::FLAG_RIVER) != 0) {
		if (!m_HashedTile.count(index)) {
			m_RiverTile.push_back(index);
			m_HashedTile.emplace(index, m_RiverTile.size() - 1);
		}
	}
}

void CFishSpawner::Spawn_Fish()
{
	if (m_RiverTile.empty())
		return;

	_uint RandIndex = Helper::Get_Random_Int(0, m_NameTable.size() - 1);
	string FishName = m_NameTable[RandIndex];
	auto iter = m_FishDataTable.find(FishName);
	if (iter == m_FishDataTable.end()) {
		m_fElapsedTime = 0.f;
		return;
	}

	CFish_Object::FishSpawnDesc* ObjDesc = new CFish_Object::FishSpawnDesc;

	ObjDesc->FishDataDesc = iter->second;
	ObjDesc->itemDataDesc = m_pItemSpawner->Get_ItemData(ObjDesc->FishDataDesc.ItemFile);
	ObjDesc->itemDataDesc.IconName = iter->second.IconName;
	ObjDesc->itemDataDesc.ItemName = iter->second.FishName;
	ObjDesc->itemDataDesc.Additionaldata = iter->second.FishFileName;

	CGameObject* pInsect =
		Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_Fish" })
		.Position(CheckRandPositon())
		.Add_ObjDesc(ObjDesc)
		.Build("Fish");

	CGameInstance::GetInstance()->Get_ObjectMgr()->Add_Object(pInsect, { "GamePlay_Level", "Fish_Layer" });
	m_fElapsedTime = 0.f;
}

_float3 CFishSpawner::CheckRandPositon()
{

	_int Max = m_RiverTile.size() - 1;
	_int Index = Helper::Get_Random_Int(0, Max);
	_float4 pos = CGameInstance::GetInstance()->Get_TileSystem()->Get_PositionByIndex(m_RiverTile[Index],ANCHOR::Center);

	return { pos.x,pos.y-5.f,pos.z };
}

FISH_DATA_DESC CFishSpawner::Get_Data(const string& insectName)
{
	auto iter = m_FishDataTable.find(insectName);

	if (iter == m_FishDataTable.end())
		return FISH_DATA_DESC();

	return iter->second;
}


ITEM_DATA_DESC CFishSpawner::Get_ItemData(const string& insectName)
{
	auto iter = m_FishDataTable.find(insectName);

	if (iter == m_FishDataTable.end())
		return ITEM_DATA_DESC();

	ITEM_DATA_DESC itemDesc = {  };
	itemDesc = m_pItemSpawner->Get_ItemData(iter->second.ItemFile);
	itemDesc.IconName = iter->second.IconName;
	itemDesc.ItemName = iter->second.FishName;
	itemDesc.Additionaldata = iter->second.FishFileName;

	return itemDesc;
}

CGameObject* CFishSpawner::Clone(INIT_DESC* pArg)
{
	CFishSpawner* instance = new CFishSpawner();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CItemSpawner");
		Safe_Release(instance);
	}

	return instance;
}


CFishSpawner* CFishSpawner::Create()
{
	CFishSpawner* instance = new CFishSpawner();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CFishSpawner");
		Safe_Release(instance);
	}

	return instance;
}

void CFishSpawner::Free()
{
	__super::Free();
	Safe_Release(m_pItemSpawner);
}