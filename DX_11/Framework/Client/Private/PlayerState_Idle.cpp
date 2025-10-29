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
	Animator->ForceChange_Animation("Base_Wait.anim",false);
}

void CPlayerState_Idle::OnUpdate(_float dt)
{
  
}

void CPlayerState_Idle::OnExit()
{
}

CState* CPlayerState_Idle::HandleTransition()
{
	CPlayer::MovementPacket tPacket = m_pPlayer->Get_MovementPacket();
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	
	 if (fabs(tPacket.vInputAxis.x) > 0 || fabs(tPacket.vInputAxis.y) > 0) {
		 if(tPacket.bRunning)
			 return m_pLayer->Get_State("Movement_Run_State");
		 else
			return m_pLayer->Get_State("Movement_Walk_State");
	}

	 auto TilePack = m_pPlayer->Get_TileInfoPacket();
	 _uint Flag = TilePack.infos[Get_Index(NEIGHBOR_INDEX::UP)].TileFlag;

	 auto ItemPack = m_pPlayer->Get_ItemPacket();

	 if (InputDev->Key_Down(VK_SPACE)) { 
		 return m_pLayer->Get_State("Action_Air_State");
		 //if ((Flag & static_cast<_uint>(TILE_FLAG::FLAG_ONITEM)) != 0) {
		//	 return m_pLayer->Get_State("Action_PickUp_State");
		 //}
		 //else if (ItemPack.CurItem.eType == ITEM_TYPE::NONE) {
		//	 return m_pLayer->Get_State("Action_Hand_State");
		 //}
		 //else if (ItemPack.CurItem.eType == ITEM_TYPE::AXE) {
		//	 return m_pLayer->Get_State("Action_Axe_State");
		 //}
		
	 }
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
