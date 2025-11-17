#include "Client_Defines.h"
#include "NpcRcm.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"
#include "MaterialAnimator.h"
#include "Texture.h"
#include "Animator3D.h"
#include "ObjectContainer.h"

#include "NpcState_Machine.h"
#include "EventSystem.h"

CNpcRcm::CNpcRcm()
{
}

CNpcRcm::CNpcRcm(const CNpcRcm& rhs)
	:CNonPlayer(rhs)
{
}

HRESULT CNpcRcm::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNpcRcm::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	m_InstanceName = "Racoon";
	return S_OK;
}

void CNpcRcm::Priority_Update(_float dt)
{
	__super::Priority_Update(dt);
}

void CNpcRcm::Update(_float dt)
{
	__super::Update(dt);
}

void CNpcRcm::Late_Update(_float dt)
{
	__super::Late_Update(dt);
}

void CNpcRcm::Render_GUI()
{
	__super::Render_GUI();
}

void CNpcRcm::Set_Closed(OnEndDialogue endMsg)
{
	__super::Set_Closed(endMsg);

	string postType = endMsg.msg.Type;

	if (postType.find("Response_") != string::npos) {
		string key = "Response_";
		string npcID = postType.substr(key.size(), postType.size());
		EVNET_NPC_TO_NPC evt = { EVENT_TYPE::Npc_To_Npc,m_CharacterDesc.NpcID, stoi(npcID), endMsg.msg.Param1 };
		m_EventPack.eventSystem->OnBroadCast<BaseEvent>(evt);
		m_EventPack.Reset();
	}

}


void CNpcRcm::Serve_Order(const string& order, _uint orderer)
{
	if (order == "GivePlayerTool") {
		m_EventPack.reservedMsg.Type = "Talking";
		m_EventPack.nextSequenceID = 2;
		m_EventPack.externalCondition = "QuestTalking";
	}
}

void CNpcRcm::EventAction(const BaseEvent& event)
{
	__super::EventAction(event);
}

CNpcRcm* CNpcRcm::Create()
{
	CNpcRcm* instance = new CNpcRcm();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CNpcRcm");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CNpcRcm::Clone(INIT_DESC* pArg)
{
	CNpcRcm* instance = new CNpcRcm(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CNpcRcm");
		Safe_Release(instance);
	}

	return instance;
}

void CNpcRcm::Free()
{
	__super::Free();
}
