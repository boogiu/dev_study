#include "Client_Defines.h"
#include"PlayerState_Axe.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_Axe::CPlayerState_Axe()
{
}

HRESULT CPlayerState_Axe::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Set_AnimationBlend("ToolAxe_APose.anim", { 19,20,21,22,23,24,25,26,27,28,29,30,31 });
	return S_OK;
}

void CPlayerState_Axe::OnUpdate(_float dt)
{
}

HRESULT CPlayerState_Axe::OnExit()
{
	return S_OK;
}

CState* CPlayerState_Axe::HandleTransition()
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

void CPlayerState_Axe::Render_State()
{
}

_uint CPlayerState_Axe::Get_InputMask() const
{
	return FlagForAction;
}

CPlayerState_Axe* CPlayerState_Axe::Create()
{
	return new CPlayerState_Axe;
}

void CPlayerState_Axe::Free()
{
	__super::Free();
}
