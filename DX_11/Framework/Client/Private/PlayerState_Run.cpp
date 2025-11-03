#include "Client_Defines.h"
#include "PlayerState_Run.h"
#include  "Player.h"
#include "Animator3D.h"
#include "Transform.h"
#include "GameInstance.h"
#include "IInputService.h"

CPlayerState_Run::CPlayerState_Run()
{
}

HRESULT CPlayerState_Run::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	HRESULT hr = Animator->Change_Animation("Move_Dash_F.anim", false, 0.05);
     m_bFliping = false;
	m_fDuration = 0;
	return hr;
}

void CPlayerState_Run::OnUpdate(_float dt)
{
	m_fDuration += dt;
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
	CPlayer::MovementPacket& tMovePacket = m_pPlayer->Get_MovementPacket();

	_float MoveSpeed = tMovePacket.fMoveSpeed;
	_float2 Player_InputAxis = tMovePacket.vInputAxis;

	if (!tMovePacket.bFliping && !m_bFliping) {
		_float2 myAxis = {};
		myAxis.x = Player_InputAxis.x * MoveSpeed * 1.5f * dt;
		myAxis.y = Player_InputAxis.y * MoveSpeed * 1.5f * dt;

		m_pPlayer->Can_Walk(myAxis);
		CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
		pTransform->Translate({ myAxis.x ,tMovePacket.fPlayerHeight * MoveSpeed * 1.5f* dt,myAxis.y });
		m_vLastAxis = Player_InputAxis;
	}

	else {
		if (m_fDuration > 1.5f) {
			Animator->Change_Animation("MoveTurn_Dash_L.anim", false, 0.01f);
			m_bFliping = true;
			_float2 breakAxis = {};

			breakAxis.x = MoveSpeed * 1.5f * m_vLastAxis.x * dt;
			breakAxis.y = MoveSpeed * 1.5f * m_vLastAxis.y * dt;

			m_pPlayer->Can_Walk(breakAxis);
			CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
			pTransform->Translate({ breakAxis.x ,tMovePacket.fPlayerHeight * MoveSpeed * 1.5f * dt,breakAxis.y });
			m_vLastAxis.x *= 0.98;
			m_vLastAxis.y *= 0.98;

			if (Animator->isCurrentAnimEnd()) {
				Animator->Change_Animation("Move_Dash_F.anim", false, 0.2);
				m_bFliping = false;
				m_fDuration = 0.f;
			}
		}
		else {
			m_fDuration = 0;
		}
	}
}

HRESULT CPlayerState_Run::OnExit()
{
	m_bFliping = false;
	return S_OK;
}

CState* CPlayerState_Run::HandleTransition()
{
	CPlayer::MovementPacket& tMovePacket = m_pPlayer->Get_MovementPacket();
	CPlayer::ControlPacket control = m_pPlayer->Get_ControlPack();
	_float2 InputAxis = tMovePacket.vInputAxis;
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (m_bFliping)
		return nullptr;
	if (!control.MsgMove)
	{
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	if (!control.MsgAdd)
	{
		return m_pLayer->Get_State("Movement_Walk_State");
	}
	
	return nullptr;
}

void CPlayerState_Run::Render_State()
{
	ImGui::Text("Last Axis : X : %.1f, Z : %.1f", m_vLastAxis.x, m_vLastAxis.y);
}

_uint CPlayerState_Run::Get_InputMask() const
{
	return FlagForMove;
}


CPlayerState_Run* CPlayerState_Run::Create()
{
	return new CPlayerState_Run;
}

void CPlayerState_Run::Free()
{
	__super::Free();
}
