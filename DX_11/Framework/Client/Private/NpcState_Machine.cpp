#include "Client_Defines.h"
#include "NpcState_Machine.h"
#include "LayerState.h"

#include "GameInstance.h"
#include "ITileService.h"
#include "IInputService.h"
#include "Animator3D.h"

#include "NpcStateHub_Idle.h"
#include "NpcStateHub_Move.h"
#include "NpcStateHub_Interact.h"

CNpcState_Machine::CNpcState_Machine(CNonPlayer* pNpc)
	:m_pOwner(pNpc)
{
}

HRESULT CNpcState_Machine::Initialize()
{
	auto actionLayer = CLayerState::Create();
	actionLayer->Set_Machine(this);
	m_LayerStates.emplace(STATE_LAYER::ACTION, actionLayer);
	
	auto Idle = actionLayer->Add_State<CNpcStateHub_Idle>("State_Hub_Idle");
	auto Move = actionLayer->Add_State<CNpcStateHub_Move>("State_Hub_Move");
	auto Interact = actionLayer->Add_State<CNpcStateHub_Interact>("State_Hub_Interact");

	Idle->Set_Owner(m_pOwner);
	Move->Set_Owner(m_pOwner);
	Interact->Set_Owner(m_pOwner);
	actionLayer->Excute(Idle);

	return S_OK;
}
void CNpcState_Machine::Update(_float dt)
{
	for (auto& LayerState : m_LayerStates) {
		if(LayerState.second)
			LayerState.second->Update(dt);
	}

	auto Animator = m_pOwner->Get_Component<CAnimator3D>();
	Animator->Update_Animation(dt);
}

void CNpcState_Machine::Request_ChangeState(STATE_LAYER eLayer, const string& NextState)
{
	auto iter = m_LayerStates.find(eLayer);
	if (iter == m_LayerStates.end())
		return;

	iter->second->Request_ChangeState(NextState);
}

void CNpcState_Machine::Render_State(CNonPlayer* pPlayer)
{
	for (auto& LayerState : m_LayerStates) {
		LayerState.second->Render_State();
	}
}

CNpcState_Machine* CNpcState_Machine::Create(CNonPlayer* pPlayer)
{
	CNpcState_Machine* instance = new CNpcState_Machine(pPlayer);
	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
	}
	return instance;
}

void CNpcState_Machine::Free()
{
	__super::Free();
}
