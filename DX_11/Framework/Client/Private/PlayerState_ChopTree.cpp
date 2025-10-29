#include "Client_Defines.h"
#include "PlayerState_ChopTree.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_ChopTree::CPlayerState_ChopTree()
{
}

void CPlayerState_ChopTree::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
 	Animator->Change_Animation("ToolAxe_Hit.anim",true);                       
	m_bInCycle = true;
}

void CPlayerState_ChopTree::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();

	if (Animator->isOverAnimTiming(0.2f)) {
		m_pPlayer->ActiveCollider_Tool(true);
	}

	if (Animator->isCurrentAnimEnd() ) {
 		m_bInCycle = false;
	}
}

void CPlayerState_ChopTree::OnExit()
{
}

CState* CPlayerState_ChopTree::HandleTransition()
{
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (!m_bInCycle) {
		m_pPlayer->ActiveCollider_Tool(false);
       		return m_pLayer->Get_State("Movement_Idle_State");
	}
	return nullptr;
}

void CPlayerState_ChopTree::Render_State()
{
}

CPlayerState_ChopTree* CPlayerState_ChopTree::Create()
{
	return new CPlayerState_ChopTree;
}

void CPlayerState_ChopTree::Free()
{
}
