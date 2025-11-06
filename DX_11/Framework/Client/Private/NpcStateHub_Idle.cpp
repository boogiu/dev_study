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
	return m_pCurrentState->OnEnter();
}

void CNpcStateHub_Idle::OnUpdate(_float dt)
{
	m_pCurrentState->OnUpdate(dt);
}

HRESULT CNpcStateHub_Idle::OnExit()
{
	return S_OK;
}

CState* CNpcStateHub_Idle::HandleTransition()
{
	return nullptr;
}

void CNpcStateHub_Idle::Render_State()
{
	ImGui::Button("Player_Near");

}

void CNpcStateHub_Idle::DecideSubState(_float dt)
{

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
