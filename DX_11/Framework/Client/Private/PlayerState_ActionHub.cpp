#include "Client_Defines.h"
#include "PlayerState_ActionHub.h"
#include "Player.h"
#include "Animator3D.h"
#include "GameInstance.h"

CPlayerState_ActionHub::CPlayerState_ActionHub()
{
}

HRESULT CPlayerState_ActionHub::OnEnter()
{
	Forward_Flag = m_pPlayer->Get_TileInfoPacket().Range_FowardInfo.TileFlag;

	return S_OK;
}

void CPlayerState_ActionHub::OnUpdate(_float dt)
{

}

HRESULT CPlayerState_ActionHub::OnExit()
{
	Forward_Flag = 0;
	return S_OK;
}

CState* CPlayerState_ActionHub::HandleTransition()
{
	return Check_ItemType();
}

CState* CPlayerState_ActionHub::Check_ItemType()
{
	CPlayer::ItemPacket tPacket = m_pPlayer->Get_ItemPacket();
	auto TilePack = m_pPlayer->Get_TileInfoPacket();
	auto infoPack = m_pPlayer->Get_InfoPack();

	CState* nextState = nullptr;

	//CheckPriority
	if (infoPack.pEncounterNpc != nullptr) {
		if (infoPack.pTalker == nullptr) {
			nextState = m_pLayer->Get_State("Action_Interact_State");
		}
	}

	if(nextState)
		return nextState;

	switch (tPacket.CurItem.TypeTag)
	{
	case itemType::None:
		if ((TILE_FLAG::FLAG_TREE & TilePack.Range_FowardInfo.TileFlag) != 0) {
			nextState = m_pLayer->Get_State("Action_TreeShake_State");
			m_pPlayer->Adjust_To_Foward();
		}
		else {
			nextState = m_pLayer->Get_State("Movement_Idle_State");
		}
		break;
	case itemType::Axe:
			nextState = m_pLayer->Get_State("Action_TreeChop_State");
			m_pPlayer->Adjust_To_Foward();
		break;
	case itemType::Scoop:
			nextState = m_pLayer->Get_State("Action_Dig_State");
			m_pPlayer->Adjust_To_Foward();
		break;
	case itemType::Net:
			nextState = m_pLayer->Get_State("Action_NetSwing_State");
		break;
	default:
		break;
	}

	return nextState;
}

CPlayerState_ActionHub* CPlayerState_ActionHub::Create()
{
	return new CPlayerState_ActionHub;
}

void CPlayerState_ActionHub::Free()
{
	__super::Free();
}
