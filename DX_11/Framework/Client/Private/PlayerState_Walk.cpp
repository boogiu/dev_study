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

void CPlayerState_Walk::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	HRESULT hr = Animator->Change_Animation("Move_Run_F.anim");
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
			pTransform->Translate({ myAxis.x ,0,myAxis.y });
		}
	}

	CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
	pTransform->Override_Rotation({ 0,1,0,0 }, XMConvertToRadians(tMovePacket.fCurrentDegree));
}

void CPlayerState_Walk::OnExit()
{
}

CState* CPlayerState_Walk::HandleTransition()
{
	CPlayer::MovementPacket tPacket = m_pPlayer->Get_MovementPacket();
	_float2 InputAxis = tPacket.vInputAxis;

	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();


	if (!isFlipping && fabs(InputAxis.x) == 0 && fabs(InputAxis.y) == 0) {
		Animator->Change_Animation("ToStop_RunLatter_L.anim");

		if (Animator->isCurrentAnimEnd()) {
			return m_pLayer->Get_State("Movement_Idle_State");
		}
	}
	else if (tPacket.bRunning) {
		return m_pLayer->Get_State("Movement_Run_State");
	}
	else {
		HRESULT hr = Animator->Change_Animation("Move_Run_F.anim");
	}
	return nullptr;
}

CPlayerState_Walk* CPlayerState_Walk::Create()
{
	return new CPlayerState_Walk;
}

void CPlayerState_Walk::Free()
{
}
