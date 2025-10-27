#include "Client_Defines.h"
#include "PlayerState_Transfer.h"
#include "Player.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_Transfer::CPlayerState_Transfer()
{
}

void CPlayerState_Transfer::OnEnter()
{
	m_bEnter = true;
	
}

void CPlayerState_Transfer::OnUpdate(_float dt)
{
	m_bEnter = false;

	/*Run Animation*/
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Update_Animation(dt);

}

void CPlayerState_Transfer::OnExit()
{
}

CState* CPlayerState_Transfer::HandleTransition()
{
	if (m_bEnter) 
		return nullptr;

	return nullptr;
}

void CPlayerState_Transfer::Render_State()
{
}


CPlayerState_Transfer* CPlayerState_Transfer::Create()
{
	return new CPlayerState_Transfer;
}

void CPlayerState_Transfer::Free()
{
	__super::Free();
}
