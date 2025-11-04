#include "Client_Defines.h"
#include "PlayerState_AimWalk.h"
#include  "Player.h"
#include "Animator3D.h"
#include "Transform.h"
#include "GameInstance.h"
#include "IInputService.h"

CPlayerState_AimWalk::CPlayerState_AimWalk()
{
}

HRESULT CPlayerState_AimWalk::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	HRESULT hr = Animator->Change_Animation("ToolNet_AimWalk_F.anim", false, 0.05);

	return hr;
}

void CPlayerState_AimWalk::OnUpdate(_float dt)
{
	CPlayer::MovementPacket tMovePacket = m_pPlayer->Get_MovementPacket();

	if (!tMovePacket.bFliping) {
		_float2 myAxis = {};

		myAxis.x = tMovePacket.vInputAxis.x * tMovePacket.fMoveSpeed *0.6* dt;
		myAxis.y = tMovePacket.vInputAxis.y * tMovePacket.fMoveSpeed *0.6* dt;

		if (m_pPlayer->Can_Walk(myAxis)) {
			CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
			pTransform->Translate({ myAxis.x ,tMovePacket.fPlayerHeight * tMovePacket.fMoveSpeed * dt,myAxis.y });
		}
	}
}

HRESULT CPlayerState_AimWalk::OnExit()
{
	return S_OK;
}

CState* CPlayerState_AimWalk::HandleTransition()
{
	CPlayer::ControlPacket control = m_pPlayer->Get_ControlPack();

	if (false == control.MsgMove) {
		return m_pLayer->Get_State("Action_NetSwing_State");
	}

	if (false == control.MsgAction) {
		return m_pLayer->Get_State("Movement_Idle_State");
	}

	return nullptr;
}

void CPlayerState_AimWalk::Render_State()
{
}

_uint CPlayerState_AimWalk::Get_InputMask() const
{
	return FlagForMove;
}


CPlayerState_AimWalk* CPlayerState_AimWalk::Create()
{
	return new CPlayerState_AimWalk;
}

void CPlayerState_AimWalk::Free()
{
	__super::Free();
}
