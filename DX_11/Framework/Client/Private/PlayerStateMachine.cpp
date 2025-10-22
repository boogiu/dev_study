#include "Client_Defines.h"
#include "PlayerStateMachine.h"
#include "PlayerHFSM.h"

#include "PlayerState_Movement.h"
#include "PlayerState_Idle.h"
#include "PlayerState_Walk.h"
#include "PlayerState_Run.h"

#include "PlayerState_Tool.h"
#include "PlayerState_Axe.h"

#include "GameInstance.h"
#include "ITileService.h"
#include "IInputService.h"
#include "Animator3D.h"

#pragma region STATE_MACHINE

CPlayerStateMachine::CPlayerStateMachine(CPlayer* pPlayer)
	:m_pOwner(pPlayer)
{
}

HRESULT CPlayerStateMachine::Initialize()
{
	m_pHFSM = CPlayerHFSM::Create(m_pOwner);

	auto Movemet = m_pHFSM->Add_State<CPlayerState_Movement>("Movement_Base_State");
	auto Idle = m_pHFSM->Add_State<CPlayerState_Idle>("Movement_Idle_State");
	auto Walk = m_pHFSM->Add_State<CPlayerState_Walk>("Movement_Walk_State");
	auto Run = m_pHFSM->Add_State<CPlayerState_Run>("Movement_Run_State");

	Idle->SetParent(Movemet);
	Walk->SetParent(Movemet);
	Run->SetParent(Movemet);

	auto Tool = m_pHFSM->Add_State<CPlayerState_Tool>("Tool_Base_State");
	auto Axe = m_pHFSM->Add_State<CPlayerState_Axe>("Tool_Axe_State");

	Axe->SetParent(Tool);

	m_pHFSM->Excute(Idle);
	return S_OK;
}

void CPlayerStateMachine::Update(_float dt)
{
	m_pHFSM->Update(dt);
}

void CPlayerStateMachine::Render_State(CPlayer* pPlayer)
{
	ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiCond_FirstUseEver);
	ImGui::Begin("Player_State");
	m_pHFSM->Render_State(pPlayer);
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
	Safe_Release(m_pHFSM);
}
#pragma endregion

