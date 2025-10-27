#include "Client_Defines.h"
#include "PlayerState_Axe.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_Axe::CPlayerState_Axe()
{
}

void CPlayerState_Axe::OnEnter()
{
              	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	auto TileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	_uint Flag = TileSystem->Get_TileFlagByIndex(m_pPlayer->Get_FowardIndex());
	isForwardTree = false;

	if ((Flag & static_cast<_uint>(TILE_FLAG::FLAG_TOOLINTERACT)) != 0) {
		if ((Flag & static_cast<_uint>(TILE_FLAG::FLAG_TREE)) != 0) {
			Animator->Chane_Animation("ToolAxe_Hit.anim");
			isForwardTree = true;
		}
		else {
			Animator->Chane_Animation("ToolAxe_Repelled.anim");
		}
	}
	else {
		Animator->Chane_Animation("ToolAxe_Air.anim");
	}
}

void CPlayerState_Axe::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();

	if (isForwardTree&&Animator->isOverAnimTiming(0.2f)) {
		m_pPlayer->ActiveCollider_Tool(true);
	}
}

void CPlayerState_Axe::OnExit()
{
}

CState* CPlayerState_Axe::HandleTransition()
{
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (Animator->isCurrentAnimEnd()) {
		m_pPlayer->ActiveCollider_Tool(false);
		return m_pHFSM->Get_State("Idle_Base_State");
	}
	return nullptr;
}

void CPlayerState_Axe::Render_State()
{
}

CPlayerState_Axe* CPlayerState_Axe::Create()
{
	return new CPlayerState_Axe;
}

void CPlayerState_Axe::Free()
{
}
