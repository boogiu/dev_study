#include "Client_Defines.h"
#include"PlayerState_Net.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_Net::CPlayerState_Net()
{
}

void CPlayerState_Net::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Set_AnimationBlend("ToolNet_APose.anim", { 19,20,21,22,23,24,25,26,27,28,29,30,31 });
}

void CPlayerState_Net::OnUpdate(_float dt)
{

}

void CPlayerState_Net::OnExit()
{

}

CState* CPlayerState_Net::HandleTransition()
{
	if (m_pPlayer->Get_InteractionPacket().isUsingTool)
		return m_pLayer->Get_State("Tool_NoTool_State");
	ITEM_TYPE nowType = m_pPlayer->Get_ItemPacket().CurItem.eType;
	if (nowType != ITEM_TYPE::NET)
	{
		return m_pLayer->Get_State("Tool_NoTool_State");
	}


	return nullptr;
}

void CPlayerState_Net::Render_State()
{
}

CPlayerState_Net* CPlayerState_Net::Create()
{
	return new CPlayerState_Net;
}

void CPlayerState_Net::Free()
{
	__super::Free();
}
