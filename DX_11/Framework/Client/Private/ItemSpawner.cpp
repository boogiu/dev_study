#include "Client_Defines.h"
#include "ItemSpawner.h"
#include "Helper_Func.h"
#include "GameInstance.h"
#include "IResourceService.h"

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
				_int typeID = { -1 };
				if (item["Type"].is_number_integer())
					typeID = item["Type"].get<_int>();
				else if (item["Type"].is_string())
					typeID = std::stoul(item["Type"].get<string>());
				data.TypeTag = static_cast<itemType>(typeID);
			}
			if (item.contains("FullCount"))
			{
				if (item["FullCount"].is_number_integer())
					data.fullCount = item["FullCount"].get<_int>(); 
				else if (item["FullCount"].is_string())
					data.fullCount = std::stoul(item["FullCount"].get<string>());
			}

			data.modelName =	   item.value("Name", "")+".model";
			data.materialName =item.value("Name", "")+".mat";
			data.ItemName = Helper::ConvertToWideString(item.value("ItemName", ""));
			data.IconName = "MenuLayout_"+item.value("IconName", "");
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
	for (auto& pair : m_ItemDataTable)
	{
		ITEM_DATA_DESC desc = pair.second;

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

