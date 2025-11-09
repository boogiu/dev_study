#include "Client_Defines.h"
#include "NpcStateHub_Iteract.h"
#include "NonPlayer.h"

#include "NpcState_Interact_SmallTalk.h"
CNpcStateHub_Iteract::CNpcStateHub_Iteract()
{
}

HRESULT CNpcStateHub_Iteract::Initialize()
{
	auto SmallTalk = Add_State(CNpcState_Interact_SmallTalk::Create(), "Interact_SamllTalk");

	return S_OK;
}

HRESULT CNpcStateHub_Iteract::OnEnter()
{
	return S_OK;
}

void CNpcStateHub_Iteract::OnUpdate(_float dt)
{
	if(m_pCurrentState)
		m_pCurrentState->OnUpdate(dt);
	DecideSubState(dt);
}

HRESULT CNpcStateHub_Iteract::OnExit()
{
	return S_OK;
}

CState* CNpcStateHub_Iteract::HandleTransition()
{
	auto EvtPack = m_pCharacter->Get_EventPack();

	if (EvtPack.HasAgenda == false&& EvtPack.playerEntrance==false) {
		return m_pLayer->Get_State("State_Hub_Idle");
	}
	return nullptr;
}

void CNpcStateHub_Iteract::DecideSubState(_float dt)
{
	//auto StatePack = {}
	auto EvtPack = m_pCharacter->Get_EventPack();

	/*가지고 있는 아젠다가 있는가? */
	if (EvtPack.HasAgenda) {

		switch (EvtPack.eAgendaType)
		{
		case AgendaType::None:
			break;
		case AgendaType::SmallTalk:
			Change_State("Interact_SamllTalk");
			break;
		case AgendaType::UpsetAbout:
			break;
		default:
			break;
		}
	}
	else {
	/*플레이어가 뭔가를 요청했는가? */
	}
}

void CNpcStateHub_Iteract::Render_State()
{
}

CNpcStateHub_Iteract* CNpcStateHub_Iteract::Create()
{
	CNpcStateHub_Iteract* instance = new CNpcStateHub_Iteract();
	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
	}
	return instance;
}

void CNpcStateHub_Iteract::Free()
{
	__super::Free();
}
