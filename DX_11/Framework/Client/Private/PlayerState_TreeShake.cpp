#include "Client_Defines.h"
#include "PlayerState_TreeShake.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_TreeShake::CPlayerState_TreeShake()
{
}

void CPlayerState_TreeShake::OnEnter()
{
	KeepTime = 0.f;
	isLooped = false;
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Chane_Animation("Tree_Shake.anim", false);
}

void CPlayerState_TreeShake::OnUpdate(_float dt)
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
			m_pPlayer->ActiveCollider_Tool(true,"KeepShake");
		}
	}
	else {
		Animator->Chane_Animation("Tree_ShakeReadyKeep.anim", false);
	}
}

void CPlayerState_TreeShake::OnExit()
{
}

CState* CPlayerState_TreeShake::HandleTransition()
{
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (!isLooped) {
		if (Animator->isCurrentAnimEnd()) {
			m_pPlayer->ActiveCollider_Tool(false);
			return m_pHFSM->Get_State("Idle_Base_State");
		}
	}
	else {
		if (InputDev->Key_Away(VK_SPACE)) {
			m_pPlayer->ActiveCollider_Tool(false);
			return m_pHFSM->Get_State("Idle_Base_State");
		}
	}
	
	return nullptr;
}

void CPlayerState_TreeShake::Render_State()
{
}

CPlayerState_TreeShake* CPlayerState_TreeShake::Create()
{
	return new CPlayerState_TreeShake;
}

void CPlayerState_TreeShake::Free()
{
}
