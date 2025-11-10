#include "Client_Defines.h"
#include "InsectSpawner.h"

#include "Helper_Func.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "Builder.h"
#include "Item_Object.h"
#include "Level.h"
#include "ItemSpawner.h"
#include "Insect_Object.h"

CInsectSpawner::CInsectSpawner()
{
}

CInsectSpawner::CInsectSpawner(const CInsectSpawner& rhs)
	:CLevelObject(rhs)
{
}

CInsectSpawner::~CInsectSpawner()
{
}

HRESULT CInsectSpawner::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CInsectSpawner::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CInsectSpawner::Priority_Update(_float dt)
{
}

void CInsectSpawner::Update(_float dt)
{
}

void CInsectSpawner::Late_Update(_float dt)
{
	m_fElapsedTime += dt;
	Update_Spawner(dt);
}

HRESULT CInsectSpawner::Link_ItemSpawner(CItemSpawner* pItemSpawner)
{
	if (m_pItemSpawner)
		Safe_Release(m_pItemSpawner);
	m_pItemSpawner = pItemSpawner;

	Safe_AddRef(m_pItemSpawner);
	return S_OK;
}

HRESULT CInsectSpawner::Read_InsectData(wstring filePath)
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


	m_InsectDataTable.clear();
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	// JSON 배열 순회
	for (auto& item : jScene)
	{
		try
		{
			string key = item.value("Name", "");
			INSECT_DATA_DESC data = {};
			if (item.contains("Index"))
			{
				if (item["Index"].is_number_integer())
					data.InsectIndex = item["Index"].get<_int>();
				else if (item["Index"].is_string())
					data.InsectIndex = std::stoul(item["Index"].get<string>());
			}

			data.InsectFileName = item.value("Name", "");
			data.InsectName = Helper::ConvertToWideString(item.value("InsectName", ""));
			data.modelName = item.value("Name", "") + ".model";
			data.materialName = item.value("Name", "") + ".mat";
			data.IconName = "MenuLayout_" + item.value("IconName", "");
			data.ItemFile =  item.value("ItemFile", "");
			data.Comment = Helper::ConvertToWideString(item.value("Comment", ""));

			if (key.empty())
				continue;

			string modelPath = item.value("model", "") + "/" + item.value("Name", "");
			string materialPath = item.value("mat", "") + "/" + item.value("Name", "");
			pRcsMgr->Add_ResourcePath(data.modelName, modelPath + ".model"); // model
			pRcsMgr->Add_ResourcePath(data.materialName, materialPath + ".mat"); // material

			m_InsectDataTable[data.InsectFileName] = data;
			_uint index = m_NameTable.size();
			m_NameTable[index] = data.InsectFileName;
		}
		catch (...)
		{
			continue;
		}
	}

	return S_OK;
}

INSECT_DATA_DESC CInsectSpawner::Get_Data(const string& insectName)
{
	auto iter = m_InsectDataTable.find(insectName);
	
	if(iter == m_InsectDataTable.end())
		return INSECT_DATA_DESC();

	return iter->second;
}

ITEM_DATA_DESC CInsectSpawner::Get_ItemData(const string& insectName)
{
	auto iter = m_InsectDataTable.find(insectName);

	if (iter == m_InsectDataTable.end())
		return ITEM_DATA_DESC();

	ITEM_DATA_DESC itemDesc = {  };
	itemDesc = m_pItemSpawner->Get_ItemData(iter->second.ItemFile);
	itemDesc.IconName = iter->second.IconName;
	itemDesc.ItemName = iter->second.InsectName;
	itemDesc.Additionaldata = iter->second.InsectFileName;

	return itemDesc;
}

CInsect_Object* CInsectSpawner::SpawnInsect(string insectName, _float4 pos)
{
	auto iter = m_InsectDataTable.find(insectName);
	if (iter == m_InsectDataTable.end()) {
		return nullptr;
	}

	CInsect_Object::InsectSpawnDesc* ObjDesc = new CInsect_Object::InsectSpawnDesc;

	ObjDesc->insectDataDesc = iter->second;
	ObjDesc->itemDataDesc = m_pItemSpawner->Get_ItemData(ObjDesc->insectDataDesc.ItemFile);
	ObjDesc->itemDataDesc.IconName = ObjDesc->insectDataDesc.IconName;
	ObjDesc->itemDataDesc.ItemName = ObjDesc->insectDataDesc.InsectName;
	ObjDesc->itemDataDesc.Additionaldata = ObjDesc->insectDataDesc.InsectFileName;
	CGameObject* pInsect =
		Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_Insect_Object" })
		.Position({ pos.x, pos.y, pos.z})
		.Add_ObjDesc(ObjDesc)
		.Build("Insect");

	CGameInstance::GetInstance()->Get_ObjectMgr()->Add_Object(pInsect, { "GamePlay_Level", "Insect_Layer" });
	return dynamic_cast<CInsect_Object*>(pInsect);
}

void CInsectSpawner::Update_Spawner(_float dt)
{

	if (m_fElapsedTime > m_fSpawnTime) {
		_uint RandIndex = Helper::Get_Random_Int(0, m_NameTable.size()-1);
		string InsectName = m_NameTable[RandIndex];


		auto iter = m_InsectDataTable.find(InsectName);
		if (iter == m_InsectDataTable.end()) {
			m_fElapsedTime = 0.f;
			return;
		}
		CInsect_Object::InsectSpawnDesc* ObjDesc = new CInsect_Object::InsectSpawnDesc;

		ObjDesc->insectDataDesc = iter->second;
		ObjDesc->itemDataDesc = m_pItemSpawner->Get_ItemData(ObjDesc->insectDataDesc.ItemFile);
		ObjDesc->itemDataDesc.IconName = ObjDesc->insectDataDesc.IconName;
		ObjDesc->itemDataDesc.ItemName = ObjDesc->insectDataDesc.InsectName;
		ObjDesc->itemDataDesc.Additionaldata = ObjDesc->insectDataDesc.InsectFileName;

		CGameObject* pInsect = 
			Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_Insect_Object" })
			.Position(CheckRandPositon())
			.Add_ObjDesc(ObjDesc)
			.Build("Insect");

		CGameInstance::GetInstance()->Get_ObjectMgr()->Add_Object(pInsect, {"GamePlay_Level", "Insect_Layer"});
		m_fElapsedTime = 0.f;
	}
}

_float3 CInsectSpawner::CheckRandPositon()
{
	_float4 TargetPos = m_pTarget->Get_Position();

	_float xRand = Helper::Get_Random_Float(10, 30);
	_float zRand = Helper::Get_Random_Float(10, 30);

	TargetPos.x += xRand;
	TargetPos.z += zRand;

	return { TargetPos.x,TargetPos.y,TargetPos.z };
}

CInsectSpawner* CInsectSpawner::Create()
{
	CInsectSpawner* instance = new CInsectSpawner();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CInsectSpawner");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CInsectSpawner::Clone(INIT_DESC* pArg)
{
	CInsectSpawner* instance = new CInsectSpawner(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CInsectSpawner");
		Safe_Release(instance);
	}

	return instance;
}

void CInsectSpawner::Free()
{
	__super::Free();
	Safe_Release(m_pItemSpawner);
}

