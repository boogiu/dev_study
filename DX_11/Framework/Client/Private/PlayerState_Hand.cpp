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
	itemType nowType = m_pPlayer->Get_ItemPacket().CurItem.TypeTag;
	switch (nowType)
	{
	case itemType::None:
		return m_pLayer->Get_State("Tool_Hand_State");
	case itemType::Axe:
		return m_pLayer->Get_State("Tool_Axe_State");
	case itemType::Scoop:
		return m_pLayer->Get_State("Tool_Scoop_State");
	case itemType::Net:
		return m_pLayer->Get_State("Tool_Net_State");
	case itemType::FishingRod:
		return m_pLayer->Get_State("Tool_Pole_State");
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
