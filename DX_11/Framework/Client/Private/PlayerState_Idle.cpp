#include "Client_Defines.h"
#include "PlayerState_Idle.h"
#include "Player.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "IInputService.h"
#include "ITileService.h"
#include "PlayerStateMachine.h"

CPlayerState_Idle::CPlayerState_Idle()
{
}

void CPlayerState_Idle::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Change_Animation("Base_Wait.anim", false);

}

void CPlayerState_Idle::OnUpdate(_float dt)
{

	ITEM_TYPE nowType = m_pPlayer->Get_ItemPacket().CurItem.eType;
	switch (nowType)
	{
	case ITEM_TYPE::NONE:
		m_pStateMachine->Request_ChangeState(STATE_LAYER::TOOL, "Tool_Hand_State");
		break;
	case ITEM_TYPE::AXE:
		m_pStateMachine->Request_ChangeState(STATE_LAYER::TOOL, "Tool_Axe_State");
		break;
	case ITEM_TYPE::SCOOP:
		m_pStateMachine->Request_ChangeState(STATE_LAYER::TOOL, "Tool_Scoop_State");
		break;
	default:
		break;
	}
}

void CPlayerState_Idle::OnExit()
{
}

CState* CPlayerState_Idle::HandleTransition()
{
	CPlayer::MovementPacket tPacket = m_pPlayer->Get_MovementPacket();
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	CPlayer::ControlPacket control = m_pPlayer->Get_ControlPack();

	if (control.MsgMove) {

		if (control.MsgDash)
			return m_pLayer->Get_State("Movement_Run_State");
		else
			return m_pLayer->Get_State("Movement_Walk_State");
	}

	auto TilePack = m_pPlayer->Get_TileInfoPacket();
	_uint Flag = TilePack.infos[Get_Index(NEIGHBOR_INDEX::UP)].TileFlag;
		Flag |= TilePack.infos[Get_Index(NEIGHBOR_INDEX::CENTER)].TileFlag;
	
	return nullptr;
}

CPlayerState_Idle* CPlayerState_Idle::Create()
{
	return new CPlayerState_Idle;
}

void CPlayerState_Idle::Free()
{
	__super::Free();
}
