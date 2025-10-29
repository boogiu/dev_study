#include "Client_Defines.h"
#include "PlayerState_HandAction.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

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
	auto pack = m_pPlayer->Get_ItemPacket();

	if (( TILE_FLAG::FLAG_TREE& Flag) != 0) {
		return m_pLayer->Get_State("Action_TreeShake_State");
	}
	else {
		return m_pLayer->Get_State("Movement_Idle_State");
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
