#include "Client_Defines.h"
#include "PlayerState_Hand.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"

CPlayerState_HandAction::CPlayerState_HandAction()
{

}

HRESULT CPlayerState_HandAction::OnEnter()
{
	return S_OK;

}

void CPlayerState_HandAction::OnUpdate(_float dt)
{

}

HRESULT CPlayerState_HandAction::OnExit()
{
	return S_OK;
}

CState* CPlayerState_HandAction::HandleTransition()
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

void CPlayerState_HandAction::Render_State()
{
}

CPlayerState_HandAction* CPlayerState_HandAction::Create()
{
	return new CPlayerState_HandAction;
}

void CPlayerState_HandAction::Free()
{
	__super::Free();
}
