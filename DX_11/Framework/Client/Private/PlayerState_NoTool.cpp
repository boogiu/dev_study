#include "Client_Defines.h"
#include"PlayerState_NoTool.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_NoTool::CPlayerState_NoTool()
{
}

void CPlayerState_NoTool::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
}

void CPlayerState_NoTool::OnUpdate(_float dt)
{
	
}

void CPlayerState_NoTool::OnExit()
{
}

CState* CPlayerState_NoTool::HandleTransition()
{
	if (m_pPlayer->Get_InteractionPacket().isUsingTool)
		return nullptr;

	ITEM_TYPE nowType = m_pPlayer->Get_ItemPacket().CurItem.eType;
	switch (nowType)
	{
	case ITEM_TYPE::NONE:
		return nullptr;
	case ITEM_TYPE::AXE:
		return m_pLayer->Get_State("Tool_Axe_State");
	case ITEM_TYPE::SCOOP:
		return m_pLayer->Get_State("Tool_Scoop_State");
		break;
	case ITEM_TYPE::NET:
		return m_pLayer->Get_State("Tool_Net_State");
		break;
	default:
		break;
	}

	return nullptr;
}

void CPlayerState_NoTool::Render_State()
{
}

CPlayerState_NoTool* CPlayerState_NoTool::Create()
{
	return new CPlayerState_NoTool;
}

void CPlayerState_NoTool::Free()
{
	__super::Free();
}
