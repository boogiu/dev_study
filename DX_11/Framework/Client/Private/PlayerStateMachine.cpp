#include "Client_Defines.h"
#include "PlayerStateMachine.h"
#include "LayerState.h"

#include "GameInstance.h"
#include "ITileService.h"
#include "IInputService.h"
#include "Animator3D.h"

#include "PlayerState_Walk.h"
#include "PlayerState_Idle.h"
#include "PlayerState_Run.h"

#include "PlayerState_AxeAction.h"
#include "PlayerState_ChopTree.h"

#include "PlayerState_HandAction.h"
#include "PlayerState_ShakeTree.h"
#include "PlayerState_TransTool.h"
#include "PlayerState_PickUp.h"
#include	 "PlayerState_Repelled.h"
#include "PlayerState_Air.h"

#include "PlayerState_ToolBase.h"
#include "PlayerState_Axe.h"
#include "PlayerState_Net.h"
#include "PlayerState_Scoop.h"
CPlayerStateMachine::CPlayerStateMachine(CPlayer* pPlayer)
	:m_pOwner(pPlayer)
{
}

HRESULT CPlayerStateMachine::Initialize()
{
	auto actionLayer = CLayerState::Create();
	actionLayer->Set_Machine(this);
	m_LayerStates.emplace(STATE_LAYER::ACTION, actionLayer);

	/*Body Action - 툴이 있건 말건 움직이는 애들 (툴의 블렌딩 상태에 영향을 받아야만 하는 애들)*/
	auto Idle = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_Idle>("Movement_Idle_State");
	auto Walk = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_Walk>("Movement_Walk_State");
	auto Run = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_Run>("Movement_Run_State");
	auto PickUP = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_PickUp>("Movement_PickUp_State");

	/*여기에 Tool 액션이란 것을 하나 넣어야 하지 않을까. -> */
	Idle->Set_Owner(m_pOwner);
	Walk->Set_Owner(m_pOwner);
	Run->Set_Owner(m_pOwner);
	PickUP->Set_Owner(m_pOwner);

	/*Tool Action - 툴에 따라 변하는 애들 / 혹은 툴의 블렌딩 여부가 상황에 따라 다른 애들*/
	auto Trans_Tool = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_TransTool>("Action_TransTool_State");
	Trans_Tool->Set_Owner(m_pOwner);

	//	/*Axe_Action*/
	//	auto AxeAction = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_AxeAction>("Action_Axe_State");
	//	auto TreeChop = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_ChopTree>("Action_TreeChop_State");
	//	
	//	/*Hand_Action*/
	//	auto HAND = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_HandAction>("Action_Hand_State");
	//	auto TreeShake = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_ShakeTree>("Action_TreeShake_State");
	//	
	//	auto Repell = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_Repelled>("Action_Repelled_State");
	//	auto Air = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_Air>("Action_Air_State");
	//	
	//	AxeAction->Set_Owner(m_pOwner);
	//	TreeChop->Set_Owner(m_pOwner);
	//	
	//	HAND->Set_Owner(m_pOwner);
	//	TreeShake->Set_Owner(m_pOwner);
	//	
	//	
	//	
	//	Repell->Set_Owner(m_pOwner);
	//	Air->Set_Owner(m_pOwner);

	auto toolLayer = CLayerState::Create();
	toolLayer->Set_Machine(this);
	m_LayerStates.emplace(STATE_LAYER::TOOL, toolLayer);

	auto ToolBase = m_LayerStates[STATE_LAYER::TOOL]->Add_State<CPlayerState_ToolBase>("Tool_Base_State");
	auto Axe= m_LayerStates[STATE_LAYER::TOOL]->Add_State<CPlayerState_Axe>("Tool_Axe_State");
	//auto Net= m_LayerStates[STATE_LAYER::TOOL]->Add_State<CPlayerState_Net>("Tool_Net_State");
	//auto Scoop= m_LayerStates[STATE_LAYER::TOOL]->Add_State<CPlayerState_Scoop>("Tool_Scoop_State");

	ToolBase->Set_Owner(m_pOwner);
	Axe->Set_Owner(m_pOwner);
	//Axe->Set_Owner(m_pOwner);
	//Net->Set_Owner(m_pOwner);
	//Scoop->Set_Owner(m_pOwner);


	m_LayerStates[STATE_LAYER::ACTION]->Excute(Idle);
	m_LayerStates[STATE_LAYER::TOOL]->Excute(ToolBase);

	return S_OK;
}

void CPlayerStateMachine::Update(_float dt)
{
	for (auto& LayerState : m_LayerStates) {
		LayerState.second->Update(dt);
	}
	auto Animator = m_pOwner->Get_Component<CAnimator3D>();
	Animator->Update_Animation(dt);
}

void CPlayerStateMachine::Request_ChangeState(STATE_LAYER eLayer, const string& NextState)
{
	auto iter = m_LayerStates.find(eLayer);
	if (iter == m_LayerStates.end())
		return;

	iter->second->Request_ChangeState(NextState);
}

void CPlayerStateMachine::Render_State(CPlayer* pPlayer)
{
	auto Animator = m_pOwner->Get_Component<CAnimator3D>();

	ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiCond_FirstUseEver);
	ImGui::Begin("Player_State");
	ImGui::SeparatorText(Animator->Get_CurrentAnimName().c_str());
	for (auto& LayerState : m_LayerStates) {
		LayerState.second->Render_State();
	}
	ImGui::End();
}

CPlayerStateMachine* CPlayerStateMachine::Create(CPlayer* pPlayer)
{
	CPlayerStateMachine* instance = new CPlayerStateMachine(pPlayer);
	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
	}
	return instance;
}

void CPlayerStateMachine::Free()
{
	__super::Free();
}

