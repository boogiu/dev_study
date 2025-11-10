#include "Client_Defines.h"
#include "NpcStateHub_Idle.h"
#include "NonPlayer.h"
#include "Animator3D.h"

#include "NpcState_Idle_Wait.h"
#include "NpcState_Idle_LookAround.h"

CNpcStateHub_Idle::CNpcStateHub_Idle()
{
}

HRESULT CNpcStateHub_Idle::Initialize()
{
	auto Wait =Add_State(CNpcState_Idle_Wait::Create(), "Idle_Wait");
	Add_State(CNpcState_Idle_LookAround::Create(), "Idle_LookAround");
	Execute(Wait);
	return S_OK;
}


HRESULT CNpcStateHub_Idle::OnEnter()
{
	auto EvtPack = m_pCharacter->Get_EventPack();
	return m_pCurrentState->OnEnter();
}

void CNpcStateHub_Idle::OnUpdate(_float dt)
{
	m_fIdleTime += dt;
	m_pCharacter->LookToPlayer(dt);

	m_pCurrentState->OnUpdate(dt);
	DecideSubState(dt);
}

HRESULT CNpcStateHub_Idle::OnExit()
{
	m_fIdleTime = 0.f;
	return  m_pCurrentState->OnExit();
}

CState* CNpcStateHub_Idle::HandleTransition()
{
	auto eventPack = m_pCharacter->Get_EventPack();

	if (eventPack.Has_Event()) {
		return m_pLayer->Get_State("State_Hub_Interact");
	}

	else if (m_fIdleTime > 10.f) {
		return m_pLayer->Get_State("State_Hub_Move");
	}
	
	return nullptr;
}

void CNpcStateHub_Idle::Render_State()
{

}

void CNpcStateHub_Idle::DecideSubState(_float dt)
{
	auto& trace = m_pCharacter->Get_TracePack();
	if (trace.Player_distance < 20)
	{
		Change_State("Idle_LookAround");
	}
	else {
		Change_State("Idle_Wait");
	}
}

CNpcStateHub_Idle* CNpcStateHub_Idle::Create()
{
	CNpcStateHub_Idle* instance = new CNpcStateHub_Idle();
	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
	}
	return instance;
}

void CNpcStateHub_Idle::Free()
{
	__super::Free();
}
