#include "Client_Defines.h"
#include "DialogueManager.h"
#include "GameInstance.h"
#include "Helper_Func.h"

#include "UI_Responcer.h"
#include "NpcSpawner.h"
#include "EventSystem.h"

#include "NonPlayer.h"

CDialogueManager::CDialogueManager()
{
}

CDialogueManager::CDialogueManager(const CDialogueManager& rhs)
	:CLevelObject(rhs)
{
}

CDialogueManager::~CDialogueManager()
{
}

HRESULT CDialogueManager::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CDialogueManager::Initialize(INIT_DESC* pArg)
{
	__super::Initialize();
	return S_OK;
}

void CDialogueManager::Priority_Update(_float dt)
{
}

void CDialogueManager::Update(_float dt)
{
}

void CDialogueManager::Late_Update(_float dt)
{
}

void CDialogueManager::Set_FreindSystem(CEventSystem* pSystem, CUI_Responcer* pUIResponcer, class CNpcSpawner* pSpawner)
{
	m_pSystem = pSystem;
	m_pUIResponcer = pUIResponcer;
	m_pNpcSpawner = pSpawner;

	Safe_AddRef(m_pSystem);
	Safe_AddRef(m_pUIResponcer);
	Safe_AddRef(m_pNpcSpawner);
	Set_EventSystem();

}

void CDialogueManager::Set_EventSystem()
{
	m_pSystem->Add_Listner<CDialogueManager, BaseEvent>(this,&CDialogueManager::Dialogue_Event);
}

void CDialogueManager::onStart_Dialogue(OnStartDialogue startMsg)
{
	CNonPlayer* speaker = dynamic_cast<CNonPlayer*>(startMsg.pSpeaker);

	if (!speaker || startMsg.SequenceID < 0)
			return;

	auto seqList = Get_SequenceData(speaker->Get_NpcData().NpcID, startMsg.SequenceID);
	if (seqList.empty())
		return;

	const string& expectedCond = seqList[0].EntryCondition;
	if (!expectedCond.empty() && expectedCond != startMsg.EntryCondition)
		return;

	m_pSystem->OnBroadCast<BaseEvent>(OnNoticeDialogue{ EVENT_TYPE::NoticeDialogue, startMsg.pSpeaker, startMsg.pListner });
	m_pSystem->OnBroadCast<BaseEvent>(OnNoticeDialogue{ EVENT_TYPE::NoticeDialogue, startMsg.pListner, startMsg.pSpeaker });

	TalkingMsgDesc desc = {};
	desc.OpenSize = { 800,160 };
	desc.OpenSpeed = 8.f;
	desc.SpeakerID = speaker->Get_NpcData().NpcID;
	desc.startSequence = startMsg.SequenceID;
	desc.SpeakerName = startMsg.SpeakerName;
	desc.pSpeaker = speaker;
	desc.OnClose = [this](OnEndDialogue endMsg){ onEnd_Dialogue(endMsg);};
	m_pUIResponcer->Active_UI("TalkingMsg", &desc);

	m_pCurrentSpeaker = startMsg.pSpeaker;
}

void CDialogueManager::Dialogue_Event(const BaseEvent& event)
{
	if (event.eType == EVENT_TYPE::DialougueStart) {
		const auto& evt = static_cast<const OnStartDialogue&>(event);
		if (evt.pSpeaker == m_pCurrentSpeaker) return;
		this->onStart_Dialogue(evt);
	}
	//	if (event.eType == EVENT_TYPE::DialougueEnd) {
	//		const auto& evt = static_cast<const OnEndDialogue&>(event);
	//		this->onEnd_Dialogue(evt);
	//	}
}

void CDialogueManager::onEnd_Dialogue(OnEndDialogue endMsg)
{
	 endMsg.pSpeaker = m_pCurrentSpeaker;
	m_pSystem->OnBroadCast<BaseEvent>(endMsg); 
	m_pCurrentSpeaker = nullptr;
}

HRESULT CDialogueManager::Read_CharacterSequece(const string& filePath)
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
			// EntryCondition
			if (item.contains("EntryCondition"))
			{
				data.EntryCondition = item.value("EntryCondition", "");
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
			data.Camera = item.value("Camera", "");

			/*강제 멈춤 시간*/
			if (item.contains("Pause"))
			{
				if (item["Pause"].is_number_float() || item["Pause"].is_number_integer())
					data.pauseTime = item["Pause"].get<_float>();
				else if (item["Pause"].is_string())
					data.pauseTime = std::stof(item["Pause"].get<string>());
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

			if (item.contains("NextSequenceID"))
			{
				if (item["NextSequenceID"].is_number_integer())
					data.NextSequenceID = item["NextSequenceID"].get<_int>();
				else if (item["NextSequenceID"].is_string())
					data.NextSequenceID = stoi(item["NextSequenceID"].get<string>());
			}
			if (item.contains("NextCondition"))
			{
				data.NextCondition = item.value("NextCondition", "");
			}

			if (item.contains("postActionMsg") && item["postActionMsg"].is_object())
			{
				auto& act = item["postActionMsg"];

				if (act.contains("Type"))
					data.postActionMsg.Type = act.value("Type", "");

				if (act.contains("Param1"))
					data.postActionMsg.Param1 = act.value("Param1", "");

				if (act.contains("Param2"))
					data.postActionMsg.Param2 = act.value("Param2", "");

				if (act.contains("NextSequenceID"))
				{
					if (act["NextSequenceID"].is_number_integer())
						data.postActionMsg.NextSequenceID = act["NextSequenceID"].get<_int>();
					else if (act["NextSequenceID"].is_string())
						data.postActionMsg.NextSequenceID = stoi(act["NextSequenceID"].get<string>());
				}
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
vector<SEQUENCE_DATA_DESC> CDialogueManager::Get_SequenceData(_int npcID, _int sequenceID)
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

CDialogueManager* CDialogueManager::Create()
{
	CDialogueManager* instance = new CDialogueManager();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CDialogueManager");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CDialogueManager::Clone(INIT_DESC* pArg)
{
	CDialogueManager* instance = new CDialogueManager(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CDialogueManager");
		Safe_Release(instance);
	}

	return instance;
}

void CDialogueManager::Free()
{
	__super::Free();

	Safe_Release(m_pSystem);
	Safe_Release(m_pUIResponcer);
	Safe_Release(m_pNpcSpawner);
}
