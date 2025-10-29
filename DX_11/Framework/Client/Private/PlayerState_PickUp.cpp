#include "Client_Defines.h"
#include "PlayerState_PickUp.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_PickUp::CPlayerState_PickUp()
{
}

void CPlayerState_PickUp::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->ForceChange_Animation("Pickup.anim", false);
}

void CPlayerState_PickUp::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	if (Animator->isOverAnimTiming(0.3)) {
		m_pPlayer->ActiveCollider_LeftHand(true, "Pick_Up");
	}
}

void CPlayerState_PickUp::OnExit()
{
}

CState* CPlayerState_PickUp::HandleTransition()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	if (Animator->isCurrentAnimEnd()) {
		m_pPlayer->ActiveCollider_LeftHand(false, "");
		return m_pLayer->Get_State("Movement_Idle_State");
	}

	return nullptr;
}

void CPlayerState_PickUp::Render_State()
{
}

CPlayerState_PickUp* CPlayerState_PickUp::Create()
{
	return new CPlayerState_PickUp;
}

void CPlayerState_PickUp::Free()
{
}
