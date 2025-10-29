#include	"Client_Defines.h"
#include "PlayerState_Tool.h"
#include "Player.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "ITileService.h"

CPlayerState_Tool::CPlayerState_Tool()
{
}

void CPlayerState_Tool::OnEnter()
{
	m_bEnter = true;
}

void CPlayerState_Tool::OnUpdate(_float dt)
{
	m_bEnter = false;
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Update_Animation(dt);
}

void CPlayerState_Tool::OnExit()
{
}

CState* CPlayerState_Tool::HandleTransition()
{
	if (m_bEnter)
		return nullptr;

	//ITEM_TYPE eType = m_pPlayer->Get_CurrentItemType();

	//TILE_INDEX forwardIndex = m_pPlayer->Get_FowardIndex();
	//auto TileSys = CGameInstance::GetInstance()->Get_TileSystem();
	//TILE_INDEX nowIndex = TileSys->Get_IndexByPosition(m_pPlayer->Get_Position());
	//TILE_INFO nowInfo = TileSys->Get_InfoByIndex(nowIndex);
	//TILE_INFO nextInfo = TileSys->Get_InfoByIndex(forwardIndex);
	//
	//if ((TILE_FLAG::FLAG_ONITEM & nowInfo.TileFlag) != 0) {
	//	return m_pLayer->Get_State("Transfer_PickUp_State");
	//}
	//if ((TILE_FLAG::FLAG_ONITEM & nextInfo.TileFlag) != 0) {
	//	return m_pLayer->Get_State("Transfer_PickUp_State");
	//}
	//else if (eType == ITEM_TYPE::NONE) {
	//	return m_pLayer->Get_State("Interaction_Base_State");
	//}
	//
	//else {
	//	switch (eType)
	//	{
	//	case ITEM_TYPE::AXE:
	//		return m_pLayer->Get_State("Tool_Axe_State");
	//		break;
	//	default:
	//		break;
	//	}
	//}
	return nullptr;
}

void CPlayerState_Tool::Render_State()
{
}

CPlayerState_Tool* CPlayerState_Tool::Create()
{
	return new CPlayerState_Tool;
}

void CPlayerState_Tool::Free()
{
	__super::Free();
}
