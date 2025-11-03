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
	itemType nowType = m_pPlayer->Get_ItemPacket().CurItem.TypeTag;
	switch (nowType)
	{
	case itemType::None:
		return m_pLayer->Get_State("Tool_Hand_State");
	case itemType::Axe:
		return m_pLayer->Get_State("Tool_Axe_State");
	case itemType::Scoop:
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
