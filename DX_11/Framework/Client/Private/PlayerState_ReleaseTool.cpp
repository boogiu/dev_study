#include "Client_Defines.h"
#include"PlayerState_ReleaseTool.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_ReleaseTool::CPlayerState_ReleaseTool()
{
}

HRESULT CPlayerState_ReleaseTool::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();	
	Animator->Release_AnimationBlend();
	Animator->Change_Animation("Generic_Putaway.anim");
	return S_OK;
}

void CPlayerState_ReleaseTool::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	if (!m_bTransed) {
		if (Animator->isOverAnimTiming(0.5f)) {
		m_pPlayer->Set_CurItemData({});
		m_bTransed = true;
	}
	}
}

HRESULT CPlayerState_ReleaseTool::OnExit()
{
	m_bTransed = false;
	return S_OK;
}

CState* CPlayerState_ReleaseTool::HandleTransition()
{
	if (m_bTransed) {
		return m_pLayer->Get_State("Tool_Hand_State");
	}
	return nullptr;
}

void CPlayerState_ReleaseTool::Render_State()
{
}

CPlayerState_ReleaseTool* CPlayerState_ReleaseTool::Create()
{
	return new CPlayerState_ReleaseTool;
}

void CPlayerState_ReleaseTool::Free()
{
	__super::Free();
}
