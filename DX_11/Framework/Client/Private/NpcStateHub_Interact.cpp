#include "Client_Defines.h"
#include "NpcStateHub_Interact.h"
#include "NonPlayer.h"

#include "NpcState_Interact_SmallTalk.h"
#include "NpcState_Interact_TransItem.h"

CNpcStateHub_Interact::CNpcStateHub_Interact()
{
}

HRESULT CNpcStateHub_Interact::Initialize()
{
	auto SmallTalk = Add_State(CNpcState_Interact_SmallTalk::Create(), "Interact_SamllTalk");
	auto Interact = Add_State(CNpcState_Interact_TransItem::Create(), "Interact_TransferItem");

	return S_OK;
}

HRESULT CNpcStateHub_Interact::OnEnter()
{
	auto EvtPack = m_pCharacter->Get_EventPack();
	return S_OK;
}

void CNpcStateHub_Interact::OnUpdate(_float dt)
{
	if(m_pCurrentState)
		m_pCurrentState->OnUpdate(dt);
	m_pCharacter->LookToPlayer(dt);
	DecideSubState(dt);
}


HRESULT CNpcStateHub_Interact::OnExit()
{

	return S_OK;
}

CState* CNpcStateHub_Interact::HandleTransition()
{
	auto& EvtPack = m_pCharacter->Get_EventPack();

	if (EvtPack.Has_Event()==false) {
		EvtPack.m_InteractCoolDown = 0.f; 
		m_pCurrentState = nullptr;
		return m_pLayer->Get_State("State_Hub_Idle");
	}
	return nullptr;
}

void CNpcStateHub_Interact::DecideSubState(_float dt)
{
	//auto StatePack = {}
	auto EvtPack = m_pCharacter->Get_EventPack();

	if (EvtPack.Has_Event()) {
		if (EvtPack.AgendaType == "SmallTalk") {
			Change_State("Interact_SamllTalk");
		}
		else if (EvtPack.AgendaType == "TransferItem") {
			Change_State("Interact_TransferItem");
		}
	}
}

void CNpcStateHub_Interact::Render_State()
{
}

CNpcStateHub_Interact* CNpcStateHub_Interact::Create()
{
	CNpcStateHub_Interact* instance = new CNpcStateHub_Interact();
	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
	}
	return instance;
}

void CNpcStateHub_Interact::Free()
{
	__super::Free();
}
