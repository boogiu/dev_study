#include "Client_Defines.h"
#include "PlayerState_Idle.h"
#include "Player.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "IInputService.h"
#include "ITileService.h"
#include "PlayerStateMachine.h"

CPlayerState_Idle::CPlayerState_Idle()
{
}

HRESULT CPlayerState_Idle::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	HRESULT hr = Animator->Change_Animation("Base_Wait.anim", true);
	if (FAILED(hr)) {
		return hr;
	}
	return hr;
}

void CPlayerState_Idle::OnUpdate(_float dt)
{

}

HRESULT CPlayerState_Idle::OnExit()
{
	return S_OK;
}

CState* CPlayerState_Idle::HandleTransition()
{
	CPlayer::MovementPacket tPacket = m_pPlayer->Get_MovementPacket();
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	CPlayer::ControlPacket control = m_pPlayer->Get_ControlPack();
	
	CState* nextState = nullptr;

	if (control.MsgBag)
	{

	}
	else if (control.MsgPickup) {
		nextState = m_pLayer->Get_State("PickUp_Base_State");
	}
	else if (control.MsgAction) {
		nextState = m_pLayer->Get_State("Action_Hub_State");
	}
	else if (control.MsgMove) {
		nextState = m_pLayer->Get_State("Movement_Walk_State");
	}
	else
		return nullptr;

	return nextState;

}

CPlayerState_Idle* CPlayerState_Idle::Create()
{
	return new CPlayerState_Idle;
}

void CPlayerState_Idle::Free()
{
	__super::Free();
}
