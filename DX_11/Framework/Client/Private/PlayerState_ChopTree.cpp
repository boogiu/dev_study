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
	/*블렌드 멈추고, 이 상태가 지속되는 동안에는 계속해서 여기*/
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Stop_AnimationBlend();

	auto TilePack = m_pPlayer->Get_TileInfoPacket();
	_uint Flag = TilePack.infos[Get_Index(NEIGHBOR_INDEX::UP)].TileFlag;

	m_bOnTree = false;

	if ((TILE_FLAG::FLAG_TREE& Flag)==0) {
		if ((TILE_FLAG::FLAG_BLOCKED & Flag) != 0) {
			Animator->Change_Animation("ToolAxe_Repelled.anim", true);
		}
		else {
			Animator->Change_Animation("ToolAxe_Air.anim", true);
		}
	}
	else if ((TILE_FLAG::FLAG_TREE & Flag) != 0) {
		Animator->Change_Animation("ToolAxe_Hit.anim", true);
		m_bOnTree = true;
	}
	m_bInCycle = true;
}

void CPlayerState_ChopTree::OnUpdate(_float dt)
{

	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();

	if (m_bOnTree) {
		if (Animator->isOverAnimTiming(0.2f)) {
			m_pPlayer->ActiveCollider_Tool(true);
		}
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
		//m_pStateMachine->Request_ChangeState(STATE_LAYER::ACTION, "Movement_Idle_State");
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	return nullptr; 
}

void CPlayerState_ChopTree::Render_State()
{
}

_uint CPlayerState_ChopTree::Get_InputMask() const
{
	return OnlyTool;
}

CPlayerState_ChopTree* CPlayerState_ChopTree::Create()
{
	return new CPlayerState_ChopTree;
}

void CPlayerState_ChopTree::Free()
{
}
