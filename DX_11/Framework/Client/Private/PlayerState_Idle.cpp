#include "Client_Defines.h"
#include "PlayerState_Idle.h"
#include "Player.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "IInputService.h"

CPlayerState_Idle::CPlayerState_Idle()
{
}

void CPlayerState_Idle::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Chane_Animation("Act_WatchCStd.anim",0.2f);
}

void CPlayerState_Idle::OnUpdate(_float dt)
{
}

void CPlayerState_Idle::OnExit()
{
}

CState* CPlayerState_Idle::HandleTransition()
{
	_float2 InputAxis = m_pPlayer->Get_InputAxis();

	if (fabs(InputAxis.x) > 0 || fabs(InputAxis.y) > 0) {
		
		if (!CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_SHIFT))
			return m_pHFSM->Get_State("Movement_Walk_State");
		else
			return m_pHFSM->Get_State("Movement_Run_State");
	}

	return nullptr;
}

CPlayerState_Idle* CPlayerState_Idle::Create()
{
	return new CPlayerState_Idle;
}

void CPlayerState_Idle::Free()
{
	__super::Free();
}
