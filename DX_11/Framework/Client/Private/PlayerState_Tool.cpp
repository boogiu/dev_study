#include	"Client_Defines.h"
#include "PlayerState_Tool.h"
#include "Player.h"
#include "Animator3D.h"
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

	ITEM_TYPE eType = m_pPlayer->Get_CurrentItemType();

	if (eType == ITEM_TYPE::NONE) {
		return m_pHFSM->Get_State("Interaction_Base_State");
	}

	else {
		switch (eType)
		{
		case ITEM_TYPE::AXE:
			return m_pHFSM->Get_State("Tool_Axe_State");
			break;
		default:
			break;
		}
	}
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
