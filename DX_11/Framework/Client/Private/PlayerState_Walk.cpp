#include "Client_Defines.h"
#include "PlayerState_Walk.h"
#include  "Player.h"
#include "Animator3D.h"
#include "Transform.h"
#include "GameInstance.h"
#include "IInputService.h"

CPlayerState_Walk::CPlayerState_Walk()
{
}

HRESULT CPlayerState_Walk::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	HRESULT hr = Animator->Change_Animation("Move_Run_F.anim");
	return hr;
}

void CPlayerState_Walk::OnUpdate(_float dt)
{
	CPlayer::MovementPacket tMovePacket = m_pPlayer->Get_MovementPacket();

	if (!tMovePacket.bFliping) {
		_float2 myAxis = {};

		myAxis.x = tMovePacket.vInputAxis.x * tMovePacket.fMoveSpeed * dt;
		myAxis.y = tMovePacket.vInputAxis.y * tMovePacket.fMoveSpeed * dt;

		if (m_pPlayer->Can_Walk(myAxis)) {
			CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
			pTransform->Translate({ myAxis.x ,tMovePacket.fPlayerHeight * tMovePacket.fMoveSpeed * dt,myAxis.y });
		}
	}

	CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
	pTransform->Override_Rotation({ 0,1,0,0 }, XMConvertToRadians(tMovePacket.fCurrentDegree));
}

HRESULT CPlayerState_Walk::OnExit()
{
	return S_OK;
}

CState* CPlayerState_Walk::HandleTransition()
{
	CPlayer::ControlPacket control = m_pPlayer->Get_ControlPack();
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (false == control.MsgMove) {
		Animator->Change_Animation("ToStop_RunLatter_L.anim",0.05f);
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	else if (control.MsgAdd) {
		return m_pLayer->Get_State("Movement_Run_State");
	}
	return nullptr;
}

_uint CPlayerState_Walk::Get_InputMask() const
{
	return FlagForMove;
}

CPlayerState_Walk* CPlayerState_Walk::Create()
{
	return new CPlayerState_Walk;
}

void CPlayerState_Walk::Free()
{
}
