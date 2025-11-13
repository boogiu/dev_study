#include "Client_Defines.h"
#include "NpcRco.h"

#include "NpcState_Machine.h"
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
	return S_OK;
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

	string postType = endMsg.msg.Type;

	if (postType.find("Order_") != string::npos) {
		string key = "Order_";
		string npcID= postType.substr(key.size(), postType.size());
		EVNET_NPC_TO_NPC evt = {m_CharacterDesc.NpcID, stoi(npcID), endMsg.msg.Param1};
		m_EventPack.eventSystem->OnBroadCast(evt);
		m_EventPack.Reset();
	}
}

void CNpcRco::Serve_Order(const string& order, _uint orderer)
{
	if (order == "GivePlayerScoop_Complete") {
		m_EventPack.Reset();
		m_EventPack.nextSequenceID = 5;
		m_EventPack.externalCondition = "NormalTalking";
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
