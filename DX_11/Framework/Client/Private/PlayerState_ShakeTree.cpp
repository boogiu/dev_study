#include "Client_Defines.h"
#include "PlayerState_ShakeTree.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_ShakeTree::CPlayerState_ShakeTree()
{
}

void CPlayerState_ShakeTree::OnEnter()
{
	KeepTime = 0.f;
	isLooped = false;
	isFinished = false;

	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Change_Animation("Tree_Shake.anim", false);

}

void CPlayerState_ShakeTree::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();

	if (InputDev->Key_Down(VK_SPACE)) {
		KeepTime += dt;
	}

	if (isLooped == false) {

		if (Animator->isOverAnimTiming(0.1f)) {
			m_pPlayer->ActiveCollider_Tool(true);
		}

		if (KeepTime > 0.5f) {
			isLooped = true;
			m_pPlayer->ActiveCollider_Tool(true, "KeepShake");
		}

	}
	else {
		Animator->Change_Animation("Tree_ShakeReadyKeep.anim", false);
		if (InputDev->Key_Away(VK_SPACE)) {
			isFinished = true;
		}
	}
}

void CPlayerState_ShakeTree::OnExit()
{
}

CState* CPlayerState_ShakeTree::HandleTransition()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	auto TilePack = m_pPlayer->Get_TileInfoPacket();
	_uint Flag = TilePack.infos[Get_Index(NEIGHBOR_INDEX::UP)].TileFlag;

	if (((Flag & static_cast<_uint>(TILE_FLAG::FLAG_TREE)) == 0)||
		Animator->isCurrentAnimEnd()														||
		isFinished) {
		m_pPlayer->ActiveCollider_Tool(false, "");
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	return nullptr;
}

void CPlayerState_ShakeTree::Render_State()
{
}

CPlayerState_ShakeTree* CPlayerState_ShakeTree::Create()
{
	return new CPlayerState_ShakeTree;
}

void CPlayerState_ShakeTree::Free()
{
	__super::Free();
}
