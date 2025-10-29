#include "Client_Defines.h"
#include "PlayerState_Repelled.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_Repelled::CPlayerState_Repelled()
{
}

void CPlayerState_Repelled::OnEnter()
{
	NoTool = false;
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	auto Packet = m_pPlayer->Get_ItemPacket();
	switch (Packet.CurItem.eType)
	{
	case ITEM_TYPE::NONE:
		NoTool = true;
		break;
	case ITEM_TYPE::AXE:
		Animator->ForceChange_Animation("ToolAxe_Repelled.anim", false);
		break;
	case ITEM_TYPE::SCOOP:
		Animator->ForceChange_Animation("ToolScoop_Repelled.anim", false);
		break;
	case ITEM_TYPE::NET:
		Animator->ForceChange_Animation("ToolNet_Swing.anim", false);
		break;
	default:
		break;
	}
}

void CPlayerState_Repelled::OnUpdate(_float dt)
{
	if (NoTool)
		return;
}

void CPlayerState_Repelled::OnExit()
{
}

CState* CPlayerState_Repelled::HandleTransition()
{
	if(NoTool)
		return m_pLayer->Get_State("Movement_Idle_State");

	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	if (Animator->isCurrentAnimEnd()) {
		return m_pLayer->Get_State("Movement_Idle_State");
	}

	return nullptr;
}

void CPlayerState_Repelled::Render_State()
{
}

CPlayerState_Repelled* CPlayerState_Repelled::Create()
{
	return new CPlayerState_Repelled;
}

void CPlayerState_Repelled::Free()
{
}
