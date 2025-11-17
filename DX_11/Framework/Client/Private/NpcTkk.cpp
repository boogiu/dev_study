#include "Client_Defines.h"
#include "NpcTkk.h"
#include "Client_Defines.h"
#include "NpcRco.h"

#include "NpcState_Machine.h"
#include "GameInstance.h"
#include "Level.h"
#include "EventSystem.h"

CNpcTkk::CNpcTkk()
{
}

CNpcTkk::CNpcTkk(const CNpcTkk& rhs)
	:CNonPlayer(rhs)
{
}

HRESULT CNpcTkk::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNpcTkk::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	m_InstanceName = "TKK";
	return S_OK;
}

void CNpcTkk::Awake()
{
	__super::Awake();
}

void CNpcTkk::Priority_Update(_float dt)
{
	__super::Priority_Update(dt);
}

void CNpcTkk::Update(_float dt)
{
	__super::Update(dt);
}

void CNpcTkk::Late_Update(_float dt)
{
	__super::Late_Update(dt);
}

void CNpcTkk::Render_GUI()
{
	__super::Render_GUI();
}

void CNpcTkk::Set_Closed(OnEndDialogue endMsg)
{
	__super::Set_Closed(endMsg);

}

void CNpcTkk::Serve_Order(const string& order, _uint orderer)
{

}

void CNpcTkk::Receive_QuestMsg(QUEST_MSG msg)
{
	
}

void CNpcTkk::EventAction(const BaseEvent& event)
{
	__super::EventAction(event);

}

CNpcTkk* CNpcTkk::Create()
{
	CNpcTkk* instance = new CNpcTkk();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CNpcTkk");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CNpcTkk::Clone(INIT_DESC* pArg)
{
	CNpcTkk* instance = new CNpcTkk(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CNpcTkk");
		Safe_Release(instance);
	}

	return instance;
}

void CNpcTkk::Free()
{
	__super::Free();
}
