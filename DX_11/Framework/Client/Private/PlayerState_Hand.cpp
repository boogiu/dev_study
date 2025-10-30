#include "Client_Defines.h"
#include "PlayerState_Hand.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"

CPlayerState_HandAction::CPlayerState_HandAction()
{

}

void CPlayerState_HandAction::OnEnter()
{

}

void CPlayerState_HandAction::OnUpdate(_float dt)
{

}

void CPlayerState_HandAction::OnExit()
{
}

CState* CPlayerState_HandAction::HandleTransition()
{
	auto TilePack = m_pPlayer->Get_TileInfoPacket();
	_uint Flag = TilePack.infos[Get_Index(NEIGHBOR_INDEX::UP)].TileFlag;
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();
	CPlayer::ControlPacket control = m_pPlayer->Get_ControlPack();

	_uint ItemFlag = Flag|TilePack.infos[Get_Index(NEIGHBOR_INDEX::CENTER)].TileFlag;

	if (control.MsgAction) {
		if ((TILE_FLAG::FLAG_ONITEM & ItemFlag) != 0) {
			m_pStateMachine->Request_ChangeState(STATE_LAYER::ACTION, "Movement_PickUp_State");
		}
		else if ((TILE_FLAG::FLAG_TREE & Flag) != 0) {
 			m_pStateMachine->Request_ChangeState(STATE_LAYER::ACTION, "Action_TreeShake_State");
		}
	}

	return nullptr;
}

void CPlayerState_HandAction::Render_State()
{
}

CPlayerState_HandAction* CPlayerState_HandAction::Create()
{
	return new CPlayerState_HandAction;
}

void CPlayerState_HandAction::Free()
{
	__super::Free();
}
