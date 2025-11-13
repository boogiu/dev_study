#include "Client_Defines.h"
#include "ItemSpawner.h"
#include "Helper_Func.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "Builder.h"
#include "Item_Object.h"
#include "Level.h"
#include "InsectSpawner.h"

CItemSpawner::CItemSpawner()
{
}

CItemSpawner::CItemSpawner(const CItemSpawner& rhs)
{
}

HRESULT CItemSpawner::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItemSpawner::Initialize(INIT_DESC* pArg)
{
	return S_OK;
}

void CItemSpawner::Priority_Update(_float dt)
{
}

void CItemSpawner::Update(_float dt)
{
}

void CItemSpawner::Late_Update(_float dt)
{
}

HRESULT CItemSpawner::Read_ItemData(wstring filePath)
{
	ifstream ifs(filePath);
	if (!ifs.is_open()) {
		MessageBoxW(nullptr, L"ItemData 파일을 찾을 수 없습니다.", L"Error", MB_OK);
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



	m_ItemDataTable.clear();
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	// JSON 배열 순회
	for (auto& item : jScene)
	{
		try
		{
			string key = item.value("Name", "");
			ITEM_DATA_DESC data = {};
			if (item.contains("Index"))
			{
				if (item["Index"].is_number_integer())
					data.ItemIndex = item["Index"].get<_int>();
				else if (item["Index"].is_string())
					data.ItemIndex = std::stoul(item["Index"].get<string>());
			}
			if (item.contains("Type"))
			{
				string type = item.value("Type", "");
				//_int typeID = { -1 };
				//if (item["Type"].is_number_integer())
				//	typeID = item["Type"].get<_int>();
				//else if (item["Type"].is_string())
				//	typeID = std::stoul(item["Type"].get<string>());
				data.TypeTag = MakeTypeByString(type);
			}
			if (item.contains("FullCount"))
			{
				if (item["FullCount"].is_number_integer())
					data.fullCount = item["FullCount"].get<_int>();
				else if (item["FullCount"].is_string())
					data.fullCount = std::stoul(item["FullCount"].get<string>());
			}
			if (item.contains("Additionaldata"))
			{
				data.Additionaldata = item.value("Additionaldata", "");
			}

			if (key == "UnitIconTentSet")
				int i = 0;

			data.modelName = item.value("Name", "") + ".model";
			data.materialName = item.value("Name", "") + ".mat";
			data.ItemName = Helper::ConvertToWideString(item.value("ItemName", ""));
			data.IconName = "MenuLayout_" + item.value("IconName", "");
			data.FileName = item.value("Name", "");

			if (key.empty())
				continue;

			string modelPath = item.value("model", "") + "/" + item.value("Name", "");
			string materialPath = item.value("mat", "") + "/" + item.value("Name", "");
			pRcsMgr->Add_ResourcePath(data.modelName, modelPath + ".model"); // model
			pRcsMgr->Add_ResourcePath(data.materialName, materialPath + ".mat"); // material
			m_ItemDataTable[data.FileName] = data;
		}
		catch (...)
		{
			continue;
		}
	}
	return S_OK;
}


ITEM_DATA_DESC CItemSpawner::Get_ItemData(string ItemTag)
{
	auto iter = m_ItemDataTable.find(ItemTag);
	if (iter == m_ItemDataTable.end()) {
		return ITEM_DATA_DESC();
	}
	else
		return iter->second;
}

CItem_Object* CItemSpawner::SpawnItem(string ItemTag, _float3 pos)
{
	CItem_Object::DROP_ITEM_DESC* pDesc = new CItem_Object::DROP_ITEM_DESC;
	pDesc->itemDesc = Get_ItemData(ItemTag);

	CGameObject* pObject =
		Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_DropItem" })
		.Add_ObjDesc(pDesc)
		.Position(pos)
		.Build(ItemTag);

	if (pObject) {
		CGameInstance::GetInstance()->Get_ObjectMgr()->Add_Object(pObject, { m_pOwner->Get_Key(),"Item_Layer" });
		return dynamic_cast<CItem_Object*>(pObject);
	}

	return nullptr;
}

CItem_Object* CItemSpawner::SpawnItem(string ItemTag)
{
	CItem_Object::DROP_ITEM_DESC* pDesc = new CItem_Object::DROP_ITEM_DESC;
	pDesc->itemDesc = Get_ItemData(ItemTag);

	CGameObject* pObject =
		Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_DropItem" })
		.Add_ObjDesc(pDesc)
		.Build(ItemTag);

	if (pObject) {
		CGameInstance::GetInstance()->Get_ObjectMgr()->Add_Object(pObject, { m_pOwner->Get_Key(),"Item_Layer" });
		return dynamic_cast<CItem_Object*>(pObject);
	}

	return nullptr;
}

CItem_Object* CItemSpawner::ThrowItem(string ItemTag, _fvector pos, _cvector MoveDir)
{
	CItem_Object::DROP_ITEM_DESC* pDesc = new CItem_Object::DROP_ITEM_DESC;
	pDesc->itemDesc = Get_ItemData(ItemTag);

	CGameObject* pObject =
		Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_DropItem" })
		.Add_ObjDesc(pDesc)
		.Build(ItemTag);

	if (pObject) {
		CGameInstance::GetInstance()->Get_ObjectMgr()->Add_Object(pObject, { m_pOwner->Get_Key(),"Item_Layer" });
		CItem_Object* item = dynamic_cast<CItem_Object*>(pObject);
		item->Set_Throw(pos, MoveDir);
		return item;
	}
	return nullptr;
}

CItem_Object* CItemSpawner::ThrowItem(ITEM_DATA_DESC data, _fvector pos, _cvector MoveDir)
{
	CItem_Object::DROP_ITEM_DESC* pDesc = new CItem_Object::DROP_ITEM_DESC;
	pDesc->itemDesc = data;

	CGameObject* pObject =
		Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_DropItem" })
		.Add_ObjDesc(pDesc)
		.Build(data.FileName);

	if (pObject) {
		CGameInstance::GetInstance()->Get_ObjectMgr()->Add_Object(pObject, { m_pOwner->Get_Key(),"Item_Layer" });
		CItem_Object* item = dynamic_cast<CItem_Object*>(pObject);
		item->Set_Throw(pos, MoveDir);
		return item;
	}
	return nullptr;
}

itemType CItemSpawner::MakeTypeByString(const string typeStr)
{
	if (typeStr == "Fruit")
		return itemType::Fruit;
	else if (typeStr == "Ore")
		return itemType::Ore;
	else if (typeStr == "Plant")
		return itemType::Plant;
	else if (typeStr == "Insect")
		return itemType::Insect;
	else if (typeStr == "Axe")
		return itemType::Axe;
	else if (typeStr == "Net")
		return itemType::Net;
	else if (typeStr == "Scoop")
		return itemType::Scoop;
	else if (typeStr == "Represent")
		return itemType::Represent;
	else if (typeStr == "Furniture")
		return itemType::Furniture;
	
	return itemType::None;
}

CItemSpawner* CItemSpawner::Create()
{
	CItemSpawner* instance = new CItemSpawner();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CItemSpawner");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CItemSpawner::Clone(INIT_DESC* pArg)
{
	CItemSpawner* instance = new CItemSpawner(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CItemSpawner");
		Safe_Release(instance);
	}

	return instance;
}

void CItemSpawner::Free()
{
	__super::Free();
}

