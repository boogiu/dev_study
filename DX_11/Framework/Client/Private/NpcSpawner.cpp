#include"Client_Defines.h"
#include "NpcSpawner.h"
#include "GameInstance.h"
#include "Helper_Func.h"
#include "IResourceService.h"
#include "IObjectService.h"
#include "NonPlayer.h"
#include "ClientHelper.h"

CNpcSpawner::CNpcSpawner()
{
}

CNpcSpawner::CNpcSpawner(const CNpcSpawner& rhs)
	:CLevelObject(rhs)
{
}

HRESULT CNpcSpawner::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CNpcSpawner::Initialize(INIT_DESC* pArg)
{
	__super::Initialize();

	return S_OK;
}

void CNpcSpawner::Priority_Update(_float dt)
{
}

void CNpcSpawner::Update(_float dt)
{
}

void CNpcSpawner::Late_Update(_float dt)
{
}

HRESULT CNpcSpawner::Read_CharacterData(const string& filePath)
{
	ifstream ifs(filePath);
	if (!ifs.is_open()) {
		MessageBoxW(nullptr, L"CharacterData 파일을 찾을 수 없습니다.", L"Error", MB_OK);
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


	m_NpcTable.clear();
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	// JSON 배열 순회
	for (auto& item : jScene)
	{
		try
		{
			NPC_DATA_DESC data = {};

			if (item.contains("Index"))
			{
				if (item["Index"].is_number_integer())
					data.NpcIndex = item["Index"].get<_int>();
				else if (item["Index"].is_string())
					data.NpcIndex = std::stoul(item["Index"].get<string>());
			}
			if (item.contains("NpcID"))
			{
				if (item["NpcID"].is_number_integer())
					data.NpcID = item["NpcID"].get<_int>();
				else if (item["NpcID"].is_string())
					data.NpcID = std::stoul(item["NpcID"].get<string>());
			}

			data.NpcName			= Helper::ConvertToWideString(item.value("NpcName", ""));
			data.ModelName		= item.value("ModelName", "");
			data.MaterialName = item.value("MaterialName", "");
			data.FolderPath		= item.value("FolderPath", "");
			data.NpcKey = item.value("NpcKey", "");
			wstring key = data.NpcName;

			if (key.empty())
				continue;

			string modelPath = data.FolderPath + "/" + data.ModelName;
			string materialPath = data.FolderPath + "/" + data.MaterialName;

			pRcsMgr->Add_ResourcePath(data.ModelName, modelPath); // model
			pRcsMgr->Add_ResourcePath(data.MaterialName, materialPath); // material

			m_NpcTable[key] = data;
		}
		catch (...)
		{
			continue;
		}
	}

	return S_OK;
}


HRESULT CNpcSpawner::Spawn_Npc(const wstring& npcName,  _float3 position, string SpcTag)
{
    auto iter = m_NpcTable.find(npcName);

    if (iter == m_NpcTable.end())
        return E_FAIL;

    CNonPlayer::NPC_SPAWN_DATA* data = new CNonPlayer::NPC_SPAWN_DATA;
    data->characterDesc = iter->second;

    CGameObject* pNpc =
        Builder::Create_Object({ "GamePlay_Level",SpcTag.empty()? "GamePlay_GameObject_NpcNrm": SpcTag })
        .Add_ObjDesc(data)
        .Position(position).Build("nPlayer");

    CGameInstance::GetInstance()->Get_ObjectMgr()->Add_Object(pNpc, { "GamePlay_Level", "NonPlayer_Layer" });

    return S_OK;
}


NPC_DATA_DESC CNpcSpawner::Get_NpcData(wstring npcName)
{
    NPC_DATA_DESC empty;
    auto iter = m_NpcTable.find(npcName);
    if (iter == m_NpcTable.end())
        return empty;

    return iter->second;
}

CNpcSpawner* CNpcSpawner::Create()
{
	CNpcSpawner* instance = new CNpcSpawner();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CNpcSpawner");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CNpcSpawner::Clone(INIT_DESC* pArg)
{
	CNpcSpawner* instance = new CNpcSpawner(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CNpcSpawner");
		Safe_Release(instance);
	}

	return instance;
}

void CNpcSpawner::Free()
{
	__super::Free();
}
