#include "Client_Defines.h"
#include "PlayerState_Interaction.h"
#include "Player.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_Interaction::CPlayerState_Interaction()
{
}

void CPlayerState_Interaction::OnEnter()
{
	m_bEnter = true;
}

void CPlayerState_Interaction::OnUpdate(_float dt)
{
	m_bEnter = false;

	/*Run Animation*/
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Update_Animation(dt);

}

void CPlayerState_Interaction::OnExit()
{
}

CState* CPlayerState_Interaction::HandleTransition()
{
	if (m_bEnter) 
		return nullptr;

	TILE_INDEX index = m_pPlayer->Get_FowardIndex();
	auto TileSys = CGameInstance::GetInstance()->Get_TileSystem();
	TILE_INFO info = TileSys->Get_InfoByIndex(index);

	if ((TILE_FLAG::FLAG_HANDINTERACT & info.TileFlag) != 0) {
		if ((TILE_FLAG::FLAG_TREE & info.TileFlag)!=0) {
 			return m_pLayer->Get_State("Interaction_TreeShake_State");
		}
	}
	else {
		return m_pLayer->Get_State("Idle_Base_State");
	}
}

void CPlayerState_Interaction::Render_State()
{
}


CPlayerState_Interaction* CPlayerState_Interaction::Create()
{
	return new CPlayerState_Interaction;
}

void CPlayerState_Interaction::Free()
{
	__super::Free();
}
