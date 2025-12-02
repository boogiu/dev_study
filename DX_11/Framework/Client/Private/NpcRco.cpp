#include "Client_Defines.h"
#include "NpcRco.h"

#include "NpcState_Machine.h"
#include "GameInstance.h"
#include "Level.h"
#include "EventSystem.h"

CNpcRco::CNpcRco()
{
}

CNpcRco::CNpcRco(const CNpcRco& rhs)
	:CNonPlayer(rhs)
{
}

HRESULT CNpcRco::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNpcRco::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	m_InstanceName = "Racoon";
	m_VoiceInt = 60;
	return S_OK;
}

void CNpcRco::Awake()
{
	__super::Awake();
}

void CNpcRco::Priority_Update(_float dt)
{
	__super::Priority_Update(dt);
}

void CNpcRco::Update(_float dt)
{
	__super::Update(dt);
}

void CNpcRco::Late_Update(_float dt)
{
	__super::Late_Update(dt);
}

void CNpcRco::Render_GUI()
{
	__super::Render_GUI();
}

void CNpcRco::Set_Closed(OnEndDialogue endMsg)
{
	__super::Set_Closed(endMsg);

	
	
}

void CNpcRco::Serve_Order(const string& order, _uint orderer)
{
	__super::Serve_Order(order, orderer);
	if (order == "GivePlayerAxe_Complete") {
		m_EventPack.Reset();
		m_EventPack.nextSequenceID = 5;
		m_EventPack.externalCondition = "NormalTalking";
	}

	if (order == "GivePlayerScoop_Complete") {
		m_EventPack.Reset();
		m_EventPack.nextSequenceID = 6;
		m_EventPack.externalCondition = "NormalTalking";
	}

	if (order == "Talk_Complete") {
		m_EventPack.Reset();
		m_EventPack.nextSequenceID = 9;
		m_EventPack.externalCondition = "QuestTalking";
	}

}

void CNpcRco::Receive_QuestMsg(QUEST_MSG msg)
{
	auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
	auto EventSys = nowLevel->Get_LevelObject<CEventSystem>();

	if (msg.eventMsg == "Event01_TreeBlock") {
		if (m_EventPack.nextSequenceID == 8) {
			m_EventPack.nextSequenceID = 9;
			EventSys->OnBroadCast<BaseEvent>(QUEST_RESPONSE{ EVENT_TYPE::Quest_Msg_Responese,msg.pQuestPublisher, true });
		}
	}
	if (msg.eventMsg == "Event02_TreeChoped") {
		if (m_EventPack.nextSequenceID == 13) {
			m_EventPack.nextSequenceID = 14;
			EventSys->OnBroadCast<BaseEvent>(QUEST_RESPONSE{ EVENT_TYPE::Quest_Msg_Responese,msg.pQuestPublisher, true });
		}
	}
}

void CNpcRco::EventAction(const BaseEvent& event)
{
	__super::EventAction(event);
	if (event.eType ==EVENT_TYPE::Quest_Msg) {
		const auto& evt = static_cast<const QUEST_MSG&>(event);
		Receive_QuestMsg(evt);
	}
}

CNpcRco* CNpcRco::Create()
{
	CNpcRco* instance = new CNpcRco();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CNpcRco");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CNpcRco::Clone(INIT_DESC* pArg)
{
	CNpcRco* instance = new CNpcRco(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CNpcRco");
		Safe_Release(instance);
	}

	return instance;
}

void CNpcRco::Free()
{
	__super::Free();
}
