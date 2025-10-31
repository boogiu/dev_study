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

HRESULT CPlayerState_PickUp::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	HRESULT hr = Animator->ForceChange_Animation("Pickup.anim", false);
	Animator->Stop_AnimationBlend();
	return hr;
}

void CPlayerState_PickUp::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (Animator->isOverAnimTiming(0.3)) {
		m_pPlayer->ActiveCollider_LeftHand(true, "Pick_Up");
	}
}

HRESULT CPlayerState_PickUp::OnExit()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Restart_AnimationBlend(); 
	return S_OK;
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

_uint CPlayerState_PickUp::Get_InputMask() const
{
	return OnlyAction;
}

CPlayerState_PickUp* CPlayerState_PickUp::Create()
{
	return new CPlayerState_PickUp;
}

void CPlayerState_PickUp::Free()
{
}
