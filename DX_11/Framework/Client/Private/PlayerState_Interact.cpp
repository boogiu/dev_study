#include "Client_Defines.h"
#include "PlayerState_Interact.h"
#include "PlayerStateMachine.h"
#include "Player.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "Builder.h"

CPlayerState_Interact::CPlayerState_Interact()
{
}

HRESULT CPlayerState_Interact::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Stop_AnimationBlend();
	Animator->Change_Animation("Base_Wait.anim", true);

	m_pPlayer->Get_Component<CCollider>()->Set_ContextEvent("WantToTalk");

	TALKING_EVENT event{ m_pPlayer, m_pPlayer->Get_InfoPack().m_pEncounterNpc , "Player" };
	m_pPlayer->BroadCast_Talk(event);
	return S_OK;
}

void CPlayerState_Interact::OnUpdate(_float dt)
{
	
}

HRESULT CPlayerState_Interact::OnExit()
{

	m_pPlayer->Get_Component<CCollider>()->Set_ContextEvent("");
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Restart_AnimationBlend();

	return S_OK;
}

CState* CPlayerState_Interact::HandleTransition()
{
	if (m_pPlayer->Get_InfoPack().m_pTalker == nullptr) {
		return m_pLayer->Get_State("Movement_Idle_State");
	}
}

void CPlayerState_Interact::Render_State()
{
}

_uint CPlayerState_Interact::Get_InputMask() const
{
	return 0;
}

void CPlayerState_Interact::OnCollisionEnter(COLLISION_CONTEXT context)
{
}

void CPlayerState_Interact::OnCollisionStay(COLLISION_CONTEXT context)
{
	
}

CPlayerState_Interact* CPlayerState_Interact::Create()
{
	return new CPlayerState_Interact;
}

void CPlayerState_Interact::Free()
{
	__super::Free();
}
