#include "Client_Defines.h"
#include "PlayerState_Eat.h"

#include "PlayerStateMachine.h"
#include "Player.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "ILevelService.h"
#include "Level.h"

CPlayerState_Eat::CPlayerState_Eat()
{
}

HRESULT CPlayerState_Eat::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Stop_AnimationBlend();
	

	Animator->Change_Animation("Menu_Eat.anim", true);
	return S_OK;
}

void CPlayerState_Eat::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	auto InfoPack = m_pPlayer->Get_InfoPack();
	if (CGameObject* pObject = InfoPack.pObjectOnLeftHand) {
		pObject->Get_Component<CModel>()->Set_CompActive(true);
		pObject->Get_Component<CTransform>()->TranslateMatrix(XMLoadFloat4x4(InfoPack.pLeftHand->Get_WorldMatrix()));

		if (Animator->isOverAnimTiming(0.7)) {
			CGameInstance::GetInstance()->Get_ObjectMgr()->Remove_Object(m_pPlayer->Get_InfoPack().pObjectOnLeftHand);
			m_pPlayer->Get_InfoPack().pObjectOnLeftHand = nullptr;
		}
	}
}

HRESULT CPlayerState_Eat::OnExit()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Restart_AnimationBlend();
	return S_OK;
}

CState* CPlayerState_Eat::HandleTransition()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (Animator->isCurrentAnimEnd())
		return m_pLayer->Get_State("Movement_Idle_State");

	return nullptr;
}

_uint CPlayerState_Eat::Get_InputMask() const
{
	return _uint();
}

void CPlayerState_Eat::Render_State()
{
}

CPlayerState_Eat* CPlayerState_Eat::Create()
{
	return new CPlayerState_Eat;
}
void CPlayerState_Eat::Free()
{
	__super::Free();
}
