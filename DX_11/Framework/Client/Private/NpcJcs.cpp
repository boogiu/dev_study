#include "Client_Defines.h"
#include "NpcJcs.h"
#include "Client_Defines.h"
#include "NpcRco.h"

#include "NpcState_Machine.h"
#include "GameInstance.h"
#include "Level.h"
#include "EventSystem.h"

#include "ClientHelper.h"
CNpcJcs::CNpcJcs()
{
}

CNpcJcs::CNpcJcs(const CNpcJcs& rhs)
	:CNonPlayer(rhs)
{
}

HRESULT CNpcJcs::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNpcJcs::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	m_InstanceName = "JCS";
	m_VoiceInt = 70;
	return S_OK;
}

void CNpcJcs::Awake()
{
	__super::Awake();
}

void CNpcJcs::Priority_Update(_float dt)
{
	__super::Priority_Update(dt);
}

void CNpcJcs::Update(_float dt)
{
	__super::Update(dt);
}

void CNpcJcs::Late_Update(_float dt)
{
	__super::Late_Update(dt);
}

void CNpcJcs::Render_GUI()
{
	__super::Render_GUI();
}

void CNpcJcs::Set_Closed(OnEndDialogue endMsg)
{
	__super::Set_Closed(endMsg);

}

void CNpcJcs::Serve_Order(const string& order, _uint orderer)
{
	__super::Serve_Order(order, orderer);
}

void CNpcJcs::Receive_QuestMsg(QUEST_MSG msg)
{
	
}

void CNpcJcs::EventAction(const BaseEvent& event)
{
	__super::EventAction(event);

}

CNpcJcs* CNpcJcs::Create()
{
	CNpcJcs* instance = new CNpcJcs();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CNpcJcs");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CNpcJcs::Clone(INIT_DESC* pArg)
{
	CNpcJcs* instance = new CNpcJcs(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CNpcJcs");
		Safe_Release(instance);
	}

	return instance;
}

void CNpcJcs::Free()
{
	__super::Free();
}
