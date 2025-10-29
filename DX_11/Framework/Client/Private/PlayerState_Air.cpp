#include "Client_Defines.h"
#include "PlayerState_Air.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_Air::CPlayerState_Air()
{
}

void CPlayerState_Air::OnEnter()
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
		Animator->ForceChange_Animation("ToolAxe_Air.anim", false);
		break;
	case ITEM_TYPE::SCOOP:
		Animator->ForceChange_Animation("ToolScoop_Air.anim", false);
		break;
	case ITEM_TYPE::NET:
		Animator->ForceChange_Animation("ToolNet_Swing.anim", false);
		break;
	default:
		break;
	}
}

void CPlayerState_Air::OnUpdate(_float dt)
{
	if (NoTool)
		return;
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (Animator->isOverAnimTiming(0.8f)) {
		//m_pPlayer->Get_InteractionPacket().isUsingTool = false;
	}
}

void CPlayerState_Air::OnExit()
{
}

CState* CPlayerState_Air::HandleTransition()
{
	if(NoTool)
		return m_pLayer->Get_State("Movement_Idle_State");

	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (Animator->isCurrentAnimEnd()) {
		auto Packet = m_pPlayer->Get_ItemPacket();
		if (Packet.CurItem.eType == ITEM_TYPE::NET) {
			Animator->Change_Animation("ToolNet_SwingStop_Ground.anim", false);
		}
		return m_pLayer->Get_State("Movement_Idle_State");
	}

	return nullptr;
}

void CPlayerState_Air::Render_State()
{
}

CPlayerState_Air* CPlayerState_Air::Create()
{
	return new CPlayerState_Air;
}

void CPlayerState_Air::Free()
{
}
