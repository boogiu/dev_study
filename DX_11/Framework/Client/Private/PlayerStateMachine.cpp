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
#include "PlayerState_AimWalk.h"

#include "PlayerState_Hand.h"
#include "PlayerState_PickUp.h"

#include "PlayerState_ToolBase.h"
#include "PlayerState_Axe.h"
#include "PlayerState_Net.h"
#include "PlayerState_Scoop.h"
#include "PlayerState_Pole.h"
#include "PlayerState_ActionHub.h"
#include "PlayerState_ShakeTree.h"
#include "PlayerState_Dig.h"
#include "PlayerState_ChopTree.h"
#include "PlayerState_NetSwing.h"
#include "PlayerState_Interact.h"

#include "PlayerState_OpenInven.h"
#include "PlayerState_TransTool.h"
#include "PlayerState_Get.h"
#include "PlayerState_Eat.h"
#include "PlayerState_Talking.h"
#include "PlayerState_TransferGet.h"
#include "PlayerState_Diy.h"
#include "PlayerState_CraftAction.h"
#include "PlayerState_PoleAction.h"

CPlayerStateMachine::CPlayerStateMachine(CPlayer* pPlayer)
	:m_pOwner(pPlayer)
{
}

HRESULT CPlayerStateMachine::Initialize()
{
	auto actionLayer = CLayerState::Create();
	actionLayer->Set_Machine(this);

	m_LayerStates.emplace(STATE_LAYER::ACTION, actionLayer);

	auto Trans_Tool = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_TransTool>("Action_TransTool_State");
	auto Open_Inven = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_OpenInven>("Action_OpenInven_State");
	auto Get = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_Get>("Action_Get_State");
	auto Eat = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_Eat>("Action_Eat_State");
	auto Craft = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_CraftAction>("Action_Craft_State");
	auto Diy = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_Diy>("Action_Diy_State");
	auto Interact = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_Interact>("Action_Interact_State");
	auto TransGet = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_TransferGet>("Action_TransGet_State");

	auto Talking = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_Talking>("Interact_Talking_State");
	Trans_Tool->Set_Owner(m_pOwner);
	Open_Inven->Set_Owner(m_pOwner);
	Get->Set_Owner(m_pOwner);
	Eat->Set_Owner(m_pOwner);
	Talking->Set_Owner(m_pOwner);
	Interact->Set_Owner(m_pOwner);
	TransGet->Set_Owner(m_pOwner);
	Diy->Set_Owner(m_pOwner);
	Craft->Set_Owner(m_pOwner);
	
	auto toolLayer = CLayerState::Create();
	toolLayer->Set_Machine(this);
	m_LayerStates.emplace(STATE_LAYER::TOOL, toolLayer);

	auto Idle = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_Idle>("Movement_Idle_State");
	auto Walk = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_Walk>("Movement_Walk_State");
	auto Run = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_Run>("Movement_Run_State");
	auto AimWalk = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_AimWalk>("Movement_AimWalk_State");

	auto PickUP = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_PickUp>("PickUp_Base_State");

	auto ActionHub = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_ActionHub>("Action_Hub_State");
	auto TreeShake = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_ShakeTree>("Action_TreeShake_State");
	auto DigAction= m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_Dig>("Action_Dig_State");
	auto TreeChop = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_ChopTree>("Action_TreeChop_State");
	auto NetSwing = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_NetSwing>("Action_NetSwing_State");
	auto PoleAction = m_LayerStates[STATE_LAYER::ACTION]->Add_State<CPlayerState_PoleAction>("Action_PoleAction_State");

	/*ToolPoseSTate*/
	auto Hand = m_LayerStates[STATE_LAYER::TOOL]->Add_State<CPlayerState_HandAction>("Tool_Hand_State");
	auto ToolBase = m_LayerStates[STATE_LAYER::TOOL]->Add_State<CPlayerState_ToolBase>("Tool_Base_State");
	auto Axe= m_LayerStates[STATE_LAYER::TOOL]->Add_State<CPlayerState_Axe>("Tool_Axe_State");
	auto Scoop= m_LayerStates[STATE_LAYER::TOOL]->Add_State<CPlayerState_Scoop>("Tool_Scoop_State");
	auto Net= m_LayerStates[STATE_LAYER::TOOL]->Add_State<CPlayerState_Net>("Tool_Net_State");
	auto Pole= m_LayerStates[STATE_LAYER::TOOL]->Add_State<CPlayerState_Pole>("Tool_Pole_State");

	Idle->Set_Owner(m_pOwner);
	Walk->Set_Owner(m_pOwner);
	Run->Set_Owner(m_pOwner);
	AimWalk->Set_Owner(m_pOwner);
	PickUP->Set_Owner(m_pOwner);
	ToolBase->Set_Owner(m_pOwner);
	Hand->Set_Owner(m_pOwner);
	TreeShake->Set_Owner(m_pOwner);
	Axe->Set_Owner(m_pOwner);
	TreeChop->Set_Owner(m_pOwner);
	Scoop->Set_Owner(m_pOwner);
	DigAction->Set_Owner(m_pOwner);
	Net->Set_Owner(m_pOwner);
	NetSwing->Set_Owner(m_pOwner);
	Pole->Set_Owner(m_pOwner);
	PoleAction->Set_Owner(m_pOwner);

	ActionHub->Set_Owner(m_pOwner);

	m_LayerStates[STATE_LAYER::TOOL]->Excute(ToolBase);
	m_LayerStates[STATE_LAYER::ACTION]->Excute(Idle);

	return S_OK;
}

void CPlayerStateMachine::Update(_float dt)
{
	for (auto& LayerState : m_LayerStates) {
		if(LayerState.second)
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


_uint CPlayerStateMachine::Get_CurrentMask(STATE_LAYER eLayer)
{
	return m_LayerStates[eLayer]->Get_CurrentMask();
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


