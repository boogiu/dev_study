#include "Client_Defines.h"
#include "PlayerState_Axe.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"

CPlayerState_Axe::CPlayerState_Axe()
{
}

void CPlayerState_Axe::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

}

void CPlayerState_Axe::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();

	if (InputDev->Key_Down(VK_SPACE)) {
		Animator->Chane_Animation("ToolAxe_Hit.anim");
	}


}

void CPlayerState_Axe::OnExit()
{
}

CState* CPlayerState_Axe::HandleTransition()
{
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if(Animator->isCurrentAnimEnd())
		return m_pHFSM->Get_State("Movement_Idle_State");

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
