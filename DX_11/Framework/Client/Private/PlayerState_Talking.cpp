#include "Client_Defines.h"
#include "PlayerState_Talking.h"
#include "Animator3D.h"
#include "GameInstance.h"
CPlayerState_Talking::CPlayerState_Talking()
{
}

HRESULT CPlayerState_Talking::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Release_AnimationBlend();
	Animator->Change_Animation("Base_Wait.anim", true);

	m_pPlayer->Adjust_To(m_pPlayer->Get_InfoPack().m_pTalker->Get_Component<CTransform>()->Get_Pos());
	m_pPlayer->Camera_Zoom_In(m_pPlayer->Get_InfoPack().m_pTalker);

	return S_OK;
}

void CPlayerState_Talking::OnUpdate(_float dt)
{
}

HRESULT CPlayerState_Talking::OnExit()
{
	m_pPlayer->Camera_Zoom_Out();

	return S_OK;
}

CState* CPlayerState_Talking::HandleTransition()
{
	if (m_pPlayer->Get_InfoPack().m_pTalker == nullptr) {
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	return nullptr;
}

void CPlayerState_Talking::Render_State()
{
}

_uint CPlayerState_Talking::Get_InputMask() const
{
	return 0;
}

CPlayerState_Talking* CPlayerState_Talking::Create()
{
	return new CPlayerState_Talking;
}

void CPlayerState_Talking::Free()
{
	__super::Free();
}
