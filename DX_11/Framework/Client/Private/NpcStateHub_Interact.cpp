#include "Client_Defines.h"
#include "NpcStateHub_Interact.h"
#include "NonPlayer.h"

#include "NpcState_Interact_SmallTalk.h"
#include "NpcState_Interact_TransItem.h"
#include "NpcState_Move_Trace.h"

#include "Player.h"
CNpcStateHub_Interact::CNpcStateHub_Interact()
{
}

HRESULT CNpcStateHub_Interact::Initialize()
{
	auto SmallTalk = Add_State(CNpcState_Interact_SmallTalk::Create(), "Interact_SamllTalk");
	auto Interact = Add_State(CNpcState_Interact_TransItem::Create(), "Interact_TransferItem");
	auto Trace = Add_State(CNpcState_Move_Trace::Create(), "Interact_Trace");

	return S_OK;
}

HRESULT CNpcStateHub_Interact::OnEnter()
{
	auto EvtPack = m_pCharacter->Get_EventPack();
	return S_OK;
}

void CNpcStateHub_Interact::OnUpdate(_float dt)
{
	if (m_pCurrentState)
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
	auto& actionPack = m_pCharacter->Get_ActionPack();
	auto& evtPack = m_pCharacter->Get_EventPack();

	if (!evtPack.isReservedAction()&& actionPack.phase == ActionPhase::None)
	{
		evtPack.m_InteractCoolDown = 0.f;
		m_pCurrentState = nullptr;
		return m_pLayer->Get_State("State_Hub_Idle");
	}

	return nullptr;
}

void CNpcStateHub_Interact::DecideSubState(_float dt)
{
	//auto StatePack = {}
	auto actionPack = m_pCharacter->Get_ActionPack();	//현재 행동상태
	auto tracePack = m_pCharacter->Get_TracePack();		// 플레이어 추적
	auto eventPack = m_pCharacter->Get_EventPack();		//예약된 행동

 	if (eventPack.isReservedAction()) {
		/*플레이어에게 가까워질 때까지*/
		if (!tracePack.Player_Near)
			Change_State("Interact_Trace");

		else {
			/*가까워졌다면, */
  			if (eventPack.reservedMsg.Type == "Talking") {
				
				if (m_pCharacter->Get_TracePack().pPlayer->Can_Talk())
					Change_State("Interact_SamllTalk");
			
			}
			else if (eventPack.reservedMsg.Type == "Transfer_Item") {
				Change_State("Interact_TransferItem");
			}
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
