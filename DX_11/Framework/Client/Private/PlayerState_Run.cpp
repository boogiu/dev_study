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
	HRESULT hr = Animator->Chane_Animation("Move_Dash_F.anim",0.05);
	m_bFliping = false;
	m_fDuration = 0;
}

void CPlayerState_Run::OnUpdate(_float dt)
{
	m_fDuration += dt;
	CPlayerState_Movement* Parent = static_cast<CPlayerState_Movement*>(m_pParent);
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();

	_float MoveSpeed = m_pPlayer->Get_MoveSpeed();
	_float2 Player_InputAxis = m_pPlayer->Get_InputAxis();

	if (!Parent->isFliping() && !m_bFliping) {
		_float2 myAxis = {};
		myAxis.x = Player_InputAxis.x * MoveSpeed * 1.5f* dt;
		myAxis.y = Player_InputAxis.y * MoveSpeed * 1.5f* dt;

		if (Parent->CheckMovable(myAxis)) {
			CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
			pTransform->Translate({ myAxis.x ,0,myAxis.y });
			m_vLastAxis= Player_InputAxis; 
		}
	}

	else {
		if (m_fDuration > 1.5f) {
			Animator->Chane_Animation("MoveTurn_Dash_L.anim", 0.1);
			m_bFliping = true;
			_float2 breakAxis = {};

			breakAxis.x = MoveSpeed* 1.5f	* m_vLastAxis.x * dt;
			breakAxis.y= MoveSpeed	* 1.5f* m_vLastAxis.y * dt;

			if (Parent->CheckMovable(breakAxis)) {
				CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
				pTransform->Translate({ breakAxis.x ,0,breakAxis.y });
				m_vLastAxis.x *= 0.98;
				m_vLastAxis.y *= 0.98;
			}

			if (Animator->isCurrentAnimEnd()) {
				Animator->Chane_Animation("Move_Dash_F.anim", 0.2);
				m_bFliping = false;
				m_fDuration = 0.f;
			}
		}
		else {
			m_fDuration = 0;
		}
	}
	_float TurnDegree = Parent->Get_CurrentDegree();
	pTransform->Override_Rotation({ 0,1,0,0 }, XMConvertToRadians(TurnDegree));
}

void CPlayerState_Run::OnExit()
{
}

CState* CPlayerState_Run::HandleTransition()
{
	_float2 InputAxis = m_pPlayer->Get_InputAxis();
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (fabs(InputAxis.x) == 0 && fabs(InputAxis.y) == 0) {
		Animator->Chane_Animation("ToStop_DashLatter_L.anim", 0.1f);
		if (Animator->isCurrentAnimEnd())
			return m_pHFSM->Get_State("Idle_Base_State");
	}
	else if (!CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_SHIFT)) {
		return m_pHFSM->Get_State("Movement_Walk_State");
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
