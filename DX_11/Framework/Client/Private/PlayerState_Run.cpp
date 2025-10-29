#include "Client_Defines.h"
#include "PlayerState_Run.h"
#include  "Player.h"
#include "Animator3D.h"
#include "Transform.h"
#include "PlayerState_Movement.h"
#include "GameInstance.h"
#include "IInputService.h"

CPlayerState_Run::CPlayerState_Run()
{
}

void CPlayerState_Run::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	HRESULT hr = Animator->ForceChange_Animation("Move_Dash_F.anim", false, 0.05);
	m_bFliping = false;
	m_fDuration = 0;
}

void CPlayerState_Run::OnUpdate(_float dt)
{
	m_fDuration += dt;
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
	CPlayer::MovementPacket& tMovePacket = m_pPlayer->Get_MovementPacket();

	_float MoveSpeed = tMovePacket.fMoveSpeed;
	_float2 Player_InputAxis = tMovePacket.vInputAxis;
	
	if (!tMovePacket.bFliping&& !m_bFliping) {
		_float2 myAxis = {};
		myAxis.x = Player_InputAxis.x * MoveSpeed * 1.5f * dt;
		myAxis.y = Player_InputAxis.y * MoveSpeed * 1.5f * dt;
	
		if (m_pPlayer->Can_Walk(myAxis)) {
			CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
			pTransform->Translate({ myAxis.x ,0,myAxis.y });
			m_vLastAxis = Player_InputAxis;
		}
	}
	
	else {
		if (m_fDuration > 1.5f) {
			Animator->Change_Animation("MoveTurn_Dash_L.anim", false, 0.01f);
			m_bFliping = true;
			_float2 breakAxis = {};
	
			breakAxis.x = MoveSpeed * 1.5f * m_vLastAxis.x * dt;
			breakAxis.y = MoveSpeed * 1.5f * m_vLastAxis.y * dt;
	
			if (m_pPlayer->Can_Walk(breakAxis)) {
				CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
				pTransform->Translate({ breakAxis.x ,0,breakAxis.y });
				m_vLastAxis.x *= 0.98;
				m_vLastAxis.y *= 0.98;
	
				if (Animator->isCurrentAnimEnd()) {
					Animator->Change_Animation("Move_Dash_F.anim", false, 0.2);
					m_bFliping = false;
					m_fDuration = 0.f;
				}
			}
		}
		else {
			m_fDuration = 0;
		}
	}
	_float TurnDegree = tMovePacket.fCurrentDegree;
	pTransform->Override_Rotation({ 0,1,0,0 }, XMConvertToRadians(TurnDegree));
}

void CPlayerState_Run::OnExit()
{
}

CState* CPlayerState_Run::HandleTransition()
{
	CPlayer::MovementPacket& tMovePacket = m_pPlayer->Get_MovementPacket();
	_float2 InputAxis = tMovePacket.vInputAxis;
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (!m_bFliping&&fabs(InputAxis.x) == 0 && fabs(InputAxis.y) == 0) {
		Animator->Change_Animation("ToStop_DashLatter_L.anim", false, 0.02f);
		return m_pLayer->Get_State("Movement_Idle_State");
	}

	if (m_bFliping && (fabs(InputAxis.x) != 0 || fabs(InputAxis.y) != 0)) {
		HRESULT hr = Animator->Change_Animation("MoveTurn_Dash_L.anim", false, 0.01f);
	}

	else if (!CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_SHIFT)) {
		return m_pLayer->Get_State("Movement_Walk_State");
	}

	return nullptr;
}

void CPlayerState_Run::Render_State()
{
	ImGui::Text("Last Axis : X : %.1f, Z : %.1f", m_vLastAxis.x, m_vLastAxis.y);
}

CPlayerState_Run* CPlayerState_Run::Create()
{
	return new CPlayerState_Run;
}

void CPlayerState_Run::Free()
{
	__super::Free();
}
