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
	m_StateContainer[m_eNowState]->Update(m_pOwner, dt);
	m_pOwner->Get_Component<CAnimator3D>()->Update_Animation(dt);
}

void CPlayerStateMachine::ChangeState(CPlayer::Player_State eNext)
{
	if (m_eNowState == eNext)
		return;
	m_StateContainer[m_eNowState]->Exit(m_pOwner);
	m_StateContainer[eNext]->Enter(m_pOwner);

	m_eNowState = eNext;
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

#pragma region IDLE_STATE
Player_IdleState::Player_IdleState(CPlayerStateMachine* pOwner)
	:m_pOwner(pOwner)
{
}

void Player_IdleState::Enter(CPlayer* pPlayer)
{
	pPlayer->Get_Component<CAnimator3D>()->Chane_Animation("Act_WatchCStd.anim");
}

void Player_IdleState::Update(CPlayer* pPlayer, _float dt)
{
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_UP)) {
		m_pOwner->ChangeState(CPlayer::Player_State::MOVE);
	}
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_DOWN)) {
		m_pOwner->ChangeState(CPlayer::Player_State::MOVE);
	}
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_RIGHT)) {
		m_pOwner->ChangeState(CPlayer::Player_State::MOVE);
	}
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_LEFT)) {
		m_pOwner->ChangeState(CPlayer::Player_State::MOVE);
	}
}

void Player_IdleState::Exit(CPlayer* pPlayer)
{
}

Player_IdleState* Player_IdleState::Create(CPlayerStateMachine* pOwner)
{
	return new Player_IdleState(pOwner);
}

#pragma endregion 

#pragma region MOVE_STATE
Player_MoveState::Player_MoveState(CPlayerStateMachine* pOwner)
	:m_pOwner(pOwner)
{
}

void Player_MoveState::Enter(CPlayer* pPlayer)
{
	pPlayer->Get_Component<CAnimator3D>()->Chane_Animation("Move_Walk_F.anim");
}

void Player_MoveState::Update(CPlayer* pPlayer, _float dt)
{
	if (
		!CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_UP)&&
		!CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_DOWN)&&
		!CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_LEFT)&&
		!CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_RIGHT)
	)
	m_pOwner->ChangeState(CPlayer::Player_State::IDLE);

}

void Player_MoveState::Exit(CPlayer* pPlayer)
{
}

Player_MoveState* Player_MoveState::Create(CPlayerStateMachine* pOwner)
{
	return new Player_MoveState(pOwner);
}
#pragma endregion 
