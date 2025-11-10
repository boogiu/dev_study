#include"Client_Defines.h"
#include "NpcSpawner.h"
#include "GameInstance.h"
#include "Helper_Func.h"
#include "IResourceService.h"
#include "IObjectService.h"
#include "NonPlayer.h"

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

HRESULT CNpcSpawner::Read_CharacterSequece(const string& filePath)
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

    m_NpcSequenceTable.clear();

    for (auto& item : jScene)
    {
        try
        {
            SEQUENCE_DATA_DESC data = {};

            // NpcID
            if (item.contains("NpcID"))
            {
                if (item["NpcID"].is_number_integer())
                    data.NpcID = item["NpcID"].get<_int>();
                else if (item["NpcID"].is_string())
                    data.NpcID = std::stoi(item["NpcID"].get<string>());
            }

            // SequenceID
            if (item.contains("SequenceID"))
            {
                if (item["SequenceID"].is_number_integer())
                    data.SequenceID = item["SequenceID"].get<_int>();
                else if (item["SequenceID"].is_string())
                    data.SequenceID = std::stoi(item["SequenceID"].get<string>());
            }

            // LineIndex
            if (item.contains("LineIndex"))
            {
                if (item["LineIndex"].is_number_integer())
                    data.LineIndex = item["LineIndex"].get<_int>();
                else if (item["LineIndex"].is_string())
                    data.LineIndex = std::stoi(item["LineIndex"].get<string>());
            }

            // 기본 대사 정보
            data.Text = Helper::ConvertToWideString(item.value("Text", ""));
            data.Emotion = item.value("Emotion", "");
            data.Motion = item.value("Motion", "");
            data.Voice = item.value("Voice", "");

            /*강제 멈춤 시간*/
            if (item.contains("Pause"))
            {
                if (item["Pause"].is_number_float() || item["Pause"].is_number_integer())
                    data.pauseTime = item["Pause"].get<_float>();
                else if (item["Pause"].is_string())
                    data.pauseTime = std::stof(item["Pause"].get<string>());
            }

            if (item.contains("PostAction") && item["PostAction"].is_object())
            {
                auto& act = item["PostAction"];
                data.postAction.Type = act.value("Type", "");
                data.postAction.Param1 = act.value("Param1", "");
                data.postAction.Param2 = act.value("Param2", 0);
                data.postAction.NextSequenceID = act.value("NextSequenceID", -1);
            }

            if (item.contains("Choices") && item["Choices"].is_array())
            {
                for (auto& choice : item["Choices"])
                {
                    CHOICE_DATA_DESC choiceData{};

                    if (choice.contains("ChoiceIndex"))
                    {
                        if (choice["ChoiceIndex"].is_number_integer())
                            choiceData.ChoiceIndex = choice["ChoiceIndex"].get<_int>();
                        else if (choice["ChoiceIndex"].is_string())
                            choiceData.ChoiceIndex = std::stoi(choice["ChoiceIndex"].get<string>());
                    }

                    choiceData.Text = Helper::ConvertToWideString(choice.value("Text", ""));
                    choiceData.NextSequenceID = choice.value("NextSequenceID", -1);

                    data.choiceSelection.push_back(choiceData);
                }

               sort(data.choiceSelection.begin(), data.choiceSelection.end(),
                    [](const CHOICE_DATA_DESC& a, const CHOICE_DATA_DESC& b)
                    {
                        return a.ChoiceIndex < b.ChoiceIndex;
                    });
            }

            _int key = data.NpcID;
            if (key < 0)
                continue;

            m_NpcSequenceTable[key][data.SequenceID].push_back(data);
        }
        catch (...)
        {
            continue;
        }
    }

    for (auto& [npcID, seqMap] : m_NpcSequenceTable)
    {
        for (auto& [seqID, lines] : seqMap)
        {
            std::sort(lines.begin(), lines.end(),
                [](const SEQUENCE_DATA_DESC& a, const SEQUENCE_DATA_DESC& b)
                {
                    return a.LineIndex < b.LineIndex;
                });
        }
    }

    return S_OK;
}

HRESULT CNpcSpawner::Spawn_Npc(const wstring& npcName,  _float3 position)
{
    auto iter = m_NpcTable.find(npcName);

    if (iter == m_NpcTable.end())
        return E_FAIL;

    CNonPlayer::NPC_SPAWN_DATA* data = new CNonPlayer::NPC_SPAWN_DATA;
    data->characterDesc = iter->second;

    CGameObject* pNpc =
        Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_NpcNrm" })
        .Add_ObjDesc(data)
        .Position(position).Build("nPlayer");

    CGameInstance::GetInstance()->Get_ObjectMgr()->Add_Object(pNpc, { "GamePlay_Level", "NonPlayer_Layer" });

    return S_OK;
}

vector<SEQUENCE_DATA_DESC> CNpcSpawner::Get_SequenceData(_int npcID, _int sequenceID)
{
    vector<SEQUENCE_DATA_DESC> empty;

    auto iter = m_NpcSequenceTable.find(npcID);
    if (iter == m_NpcSequenceTable.end())
        return empty;

    auto sequenceMap = iter->second;

    auto vecIter = sequenceMap.find(sequenceID);
    if (vecIter == sequenceMap.end())
        return empty;

    auto seqVector = vecIter->second;

    return seqVector;
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
