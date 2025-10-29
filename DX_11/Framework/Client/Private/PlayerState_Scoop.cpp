#include "Client_Defines.h"
#include"PlayerState_Scoop.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_Scoop::CPlayerState_Scoop()
{
}

void CPlayerState_Scoop::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Set_AnimationBlend("ToolScoop_APose.anim", { 19,20,21,22,23,24,25,26,27,28,29,30,31 });
}

void CPlayerState_Scoop::OnUpdate(_float dt)
{

}

void CPlayerState_Scoop::OnExit()
{

}

CState* CPlayerState_Scoop::HandleTransition()
{
	if (m_pPlayer->Get_InteractionPacket().isUsingTool)
		return m_pLayer->Get_State("Tool_NoTool_State");
	ITEM_TYPE nowType = m_pPlayer->Get_ItemPacket().CurItem.eType;
	if (nowType != ITEM_TYPE::SCOOP)
	{
		return m_pLayer->Get_State("Tool_NoTool_State");
	}


	return nullptr;
}

void CPlayerState_Scoop::Render_State()
{
}

CPlayerState_Scoop* CPlayerState_Scoop::Create()
{
	return new CPlayerState_Scoop;
}

void CPlayerState_Scoop::Free()
{
	__super::Free();
}
