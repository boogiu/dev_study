#include "Client_Defines.h"
#include "PlayerState_AxeAction.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_AxeAction::CPlayerState_AxeAction()
{

}

void CPlayerState_AxeAction::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Release_AnimationBlend();
}

void CPlayerState_AxeAction::OnUpdate(_float dt)
{
	
}

void CPlayerState_AxeAction::OnExit()
{
}

CState* CPlayerState_AxeAction::HandleTransition()
{
	auto TilePack = m_pPlayer->Get_TileInfoPacket();
	_uint Flag = TilePack.infos[Get_Index(NEIGHBOR_INDEX::UP)].TileFlag;
	auto pack = m_pPlayer->Get_ItemPacket();

	if (( TILE_FLAG::FLAG_TREE& Flag) != 0) {
		return m_pLayer->Get_State("Action_TreeChop_State");
	}
	else if ((TILE_FLAG::FLAG_STONE & Flag) != 0) {
		return m_pLayer->Get_State("Action_Repelled_State");
	}
	else {
		return m_pLayer->Get_State("Action_Air_State");
	}
	
	return nullptr;
}

void CPlayerState_AxeAction::Render_State()
{
}

CPlayerState_AxeAction* CPlayerState_AxeAction::Create()
{
	return new CPlayerState_AxeAction;
}

void CPlayerState_AxeAction::Free()
{
	__super::Free();
}
