#include "Client_Defines.h"
#include"PlayerState_ToolBase.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_ToolBase::CPlayerState_ToolBase()
{
}

HRESULT CPlayerState_ToolBase::OnEnter()
{
	return S_OK;
}

void CPlayerState_ToolBase::OnUpdate(_float dt)
{
	
}

HRESULT CPlayerState_ToolBase::OnExit()
{
	return S_OK;
}

CState* CPlayerState_ToolBase::HandleTransition()
{
	TOOL_TYPE nowType = m_pPlayer->Get_ItemPacket().CurItem.eType;
	switch (nowType)
	{
	case TOOL_TYPE::NONE:
		return m_pLayer->Get_State("Tool_Hand_State");
	case TOOL_TYPE::AXE:
		return m_pLayer->Get_State("Tool_Axe_State");
	case TOOL_TYPE::SCOOP:
		return m_pLayer->Get_State("Tool_Scoop_State"); 
	default:
		break;
	}
	return nullptr;
}

void CPlayerState_ToolBase::Render_State()
{
}

CPlayerState_ToolBase* CPlayerState_ToolBase::Create()
{
	return new CPlayerState_ToolBase;
}

void CPlayerState_ToolBase::Free()
{
	__super::Free();
}
