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

void CPlayerState_Axe::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Set_AnimationBlend("ToolAxe_APose.anim", { 19,20,21,22,23,24,25,26,27,28,29,30,31 });
}

void CPlayerState_Axe::OnUpdate(_float dt)
{

}

void CPlayerState_Axe::OnExit()
{

}

CState* CPlayerState_Axe::HandleTransition()
{
	if (m_pPlayer->Get_InteractionPacket().isUsingTool)
		return m_pLayer->Get_State("Tool_NoTool_State");

	ITEM_TYPE nowType = m_pPlayer->Get_ItemPacket().CurItem.eType;
	if (nowType != ITEM_TYPE::AXE) 
	{
		return m_pLayer->Get_State("Tool_NoTool_State");
	}

	
	return nullptr;
}

void CPlayerState_Axe::Render_State()
{
}

CPlayerState_Axe* CPlayerState_Axe::Create()
{
	return new CPlayerState_Axe;
}

void CPlayerState_Axe::Free()
{
	__super::Free();
}
