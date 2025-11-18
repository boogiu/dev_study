#include "Client_Defines.h"
#include"PlayerState_Pole.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_Pole::CPlayerState_Pole()
{
}

HRESULT CPlayerState_Pole::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Set_AnimationBlend("ToolPole_APose.anim", { 19,20,21,22,23,24,25,26,27,28,29,30,31 });
	return S_OK;
}

void CPlayerState_Pole::OnUpdate(_float dt)
{
}

HRESULT CPlayerState_Pole::OnExit()
{
	return S_OK;
}

CState* CPlayerState_Pole::HandleTransition()
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
	default:
		break;
	}
	return nullptr;
}

void CPlayerState_Pole::Render_State()
{
}

CPlayerState_Pole* CPlayerState_Pole::Create()
{
	return new CPlayerState_Pole;
}

void CPlayerState_Pole::Free()
{
	__super::Free();
}
