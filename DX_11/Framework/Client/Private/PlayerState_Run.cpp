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
	HRESULT hr = Animator->Chane_Animation("Move_Dash_F.anim");
	m_bFliping = false;
	m_fDuration = 0;
}

void CPlayerState_Run::OnUpdate(_float dt)
{
	m_fDuration += dt;
	CPlayerState_Movement* Parent = static_cast<CPlayerState_Movement*>(m_pParent);
	CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
	_float2 InputAxis = m_pPlayer->Get_InputAxis();
	_float MoveSpeed = m_pPlayer->Get_MoveSpeed();

	if (!Parent->isFliping() && !m_bFliping) {

		InputAxis.y *= MoveSpeed * 1.5f * dt;
		InputAxis.x *= MoveSpeed * 1.5f * dt;

		_float4 pos = m_pPlayer->Get_Position();
		pos.x += InputAxis.x;
		pos.z += InputAxis.y;

		if (Parent->CheckMovable({ pos.x, pos.y, pos.z,0.f })) {
			CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
			pTransform->Set_Pos({ pos.x, pos.y, pos.z });
			m_vLastAxis= m_pPlayer->Get_InputAxis();
		}

	}

	else {
		if (m_fDuration > 1.5f) {
			auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
			Animator->Chane_Animation("MoveTurn_Dash_L.anim", 0.1);
			m_bFliping = true;

			_float4  pos = m_pPlayer->Get_Position();
			pos.x += MoveSpeed * m_vLastAxis.x * dt;
			pos.z += MoveSpeed * m_vLastAxis.y * dt;

			if (Parent->CheckMovable({ pos.x, pos.y, pos.z,0.f })) {
				CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
				pTransform->Set_Pos({ pos.x, pos.y, pos.z });
				m_vLastAxis.x *= 0.95;
				m_vLastAxis.y *= 0.95;
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

	if (fabs(InputAxis.x) == 0 && fabs(InputAxis.y) == 0) {

		auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

			Animator->Chane_Animation("ToStop_DashLatter_L.anim", 0.2f);
		if (Animator->isCurrentAnimEnd())
			return m_pHFSM->Get_State("Movement_Idle_State");
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
