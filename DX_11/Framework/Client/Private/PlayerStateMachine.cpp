#include "Client_Defines.h"
#include "PlayerStateMachine.h"

#include "GameInstance.h"
#include "ITileService.h"
#include "IInputService.h"

#include "Animator3D.h"

#pragma region STATE_MACHINE
CPlayerStateMachine::CPlayerStateMachine(CPlayer* pPlayer)
	:m_pOwner(pPlayer)
{
}

HRESULT CPlayerStateMachine::Initialize()
{
	m_StateContainer.emplace(CPlayer::Player_State::IDLE, Player_IdleState::Create(this));
	m_StateContainer.emplace(CPlayer::Player_State::MOVE, Player_MoveState::Create(this));


	m_eNowState = CPlayer::Player_State::IDLE;
	return S_OK;
}

void CPlayerStateMachine::Update(_float dt)
{
	auto* pState = m_StateContainer[m_eNowState];

	if (pState->ReadyToExit(m_pOwner))
	{
 		_bool ExitComplete = pState->Exit(m_pOwner,dt);
		if (ExitComplete) {
			auto next = m_StateContainer[m_eNextState];
			next->Enter(m_pOwner);
			m_eNowState = m_eNextState;
		}
	}
	else {
		pState->Update(m_pOwner, dt);
	}

	m_pOwner->Get_Component<CAnimator3D>()->Update_Animation(dt);
}

void CPlayerStateMachine::ChangeState(CPlayer::Player_State eNext)
{
		if (m_eNowState == eNext) return;
		m_eNextState = eNext; 
}

void CPlayerStateMachine::SetInput(const _float2& moveAxis)
{
	m_vInputAxis = moveAxis;
}

string CPlayerStateMachine::GetStateName(CPlayer::Player_State eState)
{
	switch (eState)
	{
	case Client::CPlayer::Player_State::IDLE:
		return "IDLE";
	case Client::CPlayer::Player_State::MOVE:
		return "MOVE";
	default:
		break;
	}
}

void CPlayerStateMachine::Render_StateGUI(CPlayer* pPlayer)
{
	ImGui::Begin("State Machine");
	ImGui::SeparatorText("Current State");
	ImGui::Text("%s", GetStateName(m_eNowState).c_str());

	ImGui::SeparatorText("Input Axis");
	ImGui::InputFloat2("##Position", reinterpret_cast<float*>(&m_vInputAxis), "%.1f", ImGuiInputTextFlags_ReadOnly);

	ImGui::SeparatorText("Input Axis");
	ImGui::Text("Now Anim: %s", pPlayer->Get_Component<CAnimator3D>()->Get_CurrentAnimName().c_str());

	m_StateContainer[m_eNowState]->Render_StateGUI(pPlayer);
	ImGui::End();
}

CPlayerStateMachine* CPlayerStateMachine::Create(CPlayer* pPlayer)
{
	CPlayerStateMachine* instance = new CPlayerStateMachine(pPlayer);
	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
	}
	return instance;
}

void CPlayerStateMachine::Free()
{
	for (auto& pair : m_StateContainer)
		Safe_Release(pair.second);
}
#pragma endregion

IPlayerState::IPlayerState()
{
}

void IPlayerState::Render_StateGUI(CPlayer* pPlayer)
{
}

#pragma region IDLE_STATE
Player_IdleState::Player_IdleState(CPlayerStateMachine* pOwner)
	:m_pMachine(pOwner)
{
}

void Player_IdleState::Enter(CPlayer* pPlayer)
{
	m_isEndIdle = false;
}

void Player_IdleState::Update(CPlayer* pPlayer, _float dt)
{
	pPlayer->Get_Component<CAnimator3D>()->Chane_Animation("Act_WatchCStd.anim");

	const auto& moveAxis = m_pMachine->GetInputAxis();
	if (fabs(moveAxis.x) > 0.1f || fabs(moveAxis.y) > 0.1f) {
		m_pMachine->ChangeState(CPlayer::Player_State::MOVE);
		m_isEndIdle = true;
	}
}

_bool Player_IdleState::Exit(CPlayer* pPlayer, _float dt)
{
	return true;
}

_bool Player_IdleState::ReadyToExit(CPlayer* pPlayer)
{
	return m_isEndIdle;
}

Player_IdleState* Player_IdleState::Create(CPlayerStateMachine* pMachine)
{
	return new Player_IdleState(pMachine);
}
#pragma endregion 

#pragma region MOVE_STATE
Player_MoveState::Player_MoveState(CPlayerStateMachine* pMachine)
	:m_pMachine(pMachine)
{
}

void Player_MoveState::Enter(CPlayer* pPlayer)
{
	m_isMoveEnd = false;
}

void Player_MoveState::Update(CPlayer* pPlayer, _float dt)
{
	_float2 moveAxis = m_pMachine->GetInputAxis();

	if (fabs(moveAxis.x) < 0.1f && fabs(moveAxis.y) < 0.1f)
	{
		m_pMachine->ChangeState(CPlayer::Player_State::IDLE);
		m_isMoveEnd = true;
	}

	CTransform* pTransform = pPlayer->Get_Component<CTransform>();
	string animName = {};

	/*입력된 값에 따른 각도 구하기.*/
	_float fTargetRadian = atan2(moveAxis.x, moveAxis.y);
	_float fTargetDegree = XMConvertToDegrees(fTargetRadian);

	//남은 각도 구하기.
	_float fDeltaDegree = fTargetDegree - m_fNowDegree;
	if (fDeltaDegree > 180.f)  fDeltaDegree -= 360.f; //(-180~180)
	if (fDeltaDegree < -180.f) fDeltaDegree += 360.f;
	animName = "Move_Run_F.anim";

	//만약 남은 각도가 160도 이상이면 플립
	if (fabs(fDeltaDegree) > 179.f) {
		isFlipping = true;
	}

	/*회전은 누적해서 덮어쓰기*/
	pTransform->Override_Rotation({ 0,1,0,0 }, XMConvertToRadians(m_fNowDegree));
	float fTurnSpeed =360.f * dt;
	if (fabs(fDeltaDegree) < 0.1f) {
		m_fNowDegree = fTargetDegree;
		isFlipping = false;
	}
	else
		m_fNowDegree += (fDeltaDegree > 0 ? 1.f : -1.f) * fTurnSpeed;

	if (!isFlipping) {
		moveAxis.y *= m_fMoveSpeed * dt;
		moveAxis.x *= m_fMoveSpeed * dt;
		_float4 pos = pPlayer->Get_Position();
		pos.x += moveAxis.x;
		pos.z += moveAxis.y;
		pTransform->Set_Pos({ pos.x, pos.y, pos.z });
	}

	pPlayer->Get_Component<CAnimator3D>()->Chane_Animation(animName);


}


_bool Player_MoveState::Exit(CPlayer* pPlayer, _float dt)
{
	pPlayer->Get_Component<CAnimator3D>()->Chane_Animation("ToStop_RunFirst_L.anim", 0.05f);
	if (pPlayer->Get_Component<CAnimator3D>()->isCurrentAnimEnd()) {
		return true;
	}
	return false;
}

_bool Player_MoveState::ReadyToExit(CPlayer* pPlayer)
{
	return m_isMoveEnd;
}

void Player_MoveState::Render_StateGUI(CPlayer* pPlayer)
{
	ImGui::Text("Now Degree: %f", m_fNowDegree);
}

Player_MoveState* Player_MoveState::Create(CPlayerStateMachine* pMachine)
{
	return new Player_MoveState(pMachine);
}
#pragma endregion 
