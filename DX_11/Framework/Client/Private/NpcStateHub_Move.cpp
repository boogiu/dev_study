#include "Client_Defines.h"
#include "NpcStateHub_Move.h"
#include "NonPlayer.h"
#include "Animator3D.h"

#include "LayerState.h"
#include "NpcState_Move_Walk.h"
#include "NpcState_Move_Trace.h"

CNpcStateHub_Move::CNpcStateHub_Move()
{
}

HRESULT CNpcStateHub_Move::Initialize()
{
	auto Walk = Add_State(CNpcState_Move_Walk::Create(), "Move_Walk");
	auto Trace = Add_State(CNpcState_Move_Trace::Create(), "Move_Trace");
	Execute(Walk);
	return S_OK;
}


HRESULT CNpcStateHub_Move::OnEnter()
{
	return m_pCurrentState->OnEnter();
}

void CNpcStateHub_Move::OnUpdate(_float dt)
{
	m_fMoveTime += dt;
	m_pCurrentState->OnUpdate(dt);
	DecideSubState(dt);
}

HRESULT CNpcStateHub_Move::OnExit()
{
	m_fMoveTime = 0.f;
	return m_pCurrentState->OnExit();
}

CState* CNpcStateHub_Move::HandleTransition()
{
	auto tracePack = m_pCharacter->Get_TracePack();
	auto eventPack = m_pCharacter->Get_EventPack();

	if (m_fMoveTime > 50.f) {
		return m_pLayer->Get_State("State_Hub_Idle");
	}
	if (tracePack.Player_Near) {
		if(eventPack.HasAgenda) /*용건이 있으면 상호작용으로. */
			return m_pLayer->Get_State("State_Hub_Interact");
		else
			return m_pLayer->Get_State("State_Hub_Idle");
	}
	return nullptr;
}

void CNpcStateHub_Move::Render_State()
{
}

void CNpcStateHub_Move::DecideSubState(_float dt)
{
	auto tracePack = m_pCharacter->Get_TracePack();
	auto eventPack = m_pCharacter->Get_EventPack();

	if (eventPack.HasAgenda)
	{
		Change_State("Move_Trace");
	}
	else
	{
		Change_State("Move_Walk");
	}
}


CNpcStateHub_Move* CNpcStateHub_Move::Create()
{
	CNpcStateHub_Move* instance = new CNpcStateHub_Move();
	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
	}
	return instance;
}

void CNpcStateHub_Move::Free()
{
	__super::Free();
}
