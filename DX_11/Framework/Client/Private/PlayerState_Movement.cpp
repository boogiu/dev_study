#include "Client_Defines.h"
#include "PlayerState_Movement.h"
#include "Player.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "IInputService.h"
CPlayerState_Movement::CPlayerState_Movement()
{
}

void CPlayerState_Movement::OnEnter()
{
	m_bEnter = true;
}

void CPlayerState_Movement::OnUpdate(_float dt)
{
	m_bEnter = false;
	/*Run Animation*/
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Update_Animation(dt);

	/*Check Radian*/
	_float2 InputAxis = m_pPlayer->Get_InputAxis();
	if (fabs(InputAxis.x) > 0.01f || fabs(InputAxis.y) > 0.01f)
	{
		m_fTargetDegree = XMConvertToDegrees(atan2(InputAxis.x, InputAxis.y));
	}

	_float DeltaDegree = m_fTargetDegree - m_fTurnDegree;

	// -180~180 범위로 정규화
	while (DeltaDegree > 180.f) DeltaDegree -= 360.f;
	while (DeltaDegree < -180.f) DeltaDegree += 360.f;

	_float RotSpeed =  dt * 480;

	if(fabs(DeltaDegree) > 150.f)
		m_bFliping = true;

	if (fabs(DeltaDegree) > 5.f) {
		if (m_bFliping)
			m_fTurnDegree -= RotSpeed;
		else
		m_fTurnDegree += (DeltaDegree > 0 ? RotSpeed : -RotSpeed);
	}

	else {
		m_bFliping = false;
		m_fTurnDegree = m_fTargetDegree; 
	}
}

void CPlayerState_Movement::OnExit()
{
}

CState* CPlayerState_Movement::HandleTransition()
{
	if (m_bEnter) 
		return nullptr;

	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_SPACE)) {
		return m_pHFSM->Get_State("Tool_Base_State");
	};

	return nullptr;
}

_bool CPlayerState_Movement::CheckMovable(_float4 nextPos)
{
	return true;
}

void CPlayerState_Movement::Render_State()
{
	_float2 InputAxis = m_pPlayer->Get_InputAxis();
	ImGui::Text("Axis : X : %.1f, Z : %.1f", InputAxis.x, InputAxis.y);
	ImGui::Text("Target Degree : %.2f", m_fTargetDegree);
	ImGui::Text("Current Degree : %.2f", m_fTurnDegree);
	ImGui::Checkbox("Flipping", &m_bFliping);
}

CPlayerState_Movement* CPlayerState_Movement::Create()
{
	return new CPlayerState_Movement;
}

void CPlayerState_Movement::Free()
{
	__super::Free();
}
