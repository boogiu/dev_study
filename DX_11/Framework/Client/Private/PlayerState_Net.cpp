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

HRESULT CPlayerState_Net::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Set_AnimationBlend("ToolNet_APose.anim", { 19,20,21,22,23,24,25,26,27,28,29,30,31 });
	return S_OK;
}

void CPlayerState_Net::OnUpdate(_float dt)
{

}

HRESULT CPlayerState_Net::OnExit()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Release_AnimationBlend();
	return S_OK;
}

CState* CPlayerState_Net::HandleTransition()
{
	TOOL_TYPE nowType = m_pPlayer->Get_ItemPacket().CurItem.eType;
	if (nowType != TOOL_TYPE::NET)
	{
		return m_pLayer->Get_State("Tool_Base_State");
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
