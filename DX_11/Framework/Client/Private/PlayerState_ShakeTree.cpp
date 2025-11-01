#include "Client_Defines.h"
#include "PlayerState_ShakeTree.h"
#include "PlayerStateMachine.h"
#include "Player.h"
#include "Animator3D.h"
#include "GameInstance.h"

CPlayerState_ShakeTree::CPlayerState_ShakeTree()
{
}

HRESULT CPlayerState_ShakeTree::OnEnter()
{
	KeepTime = 0.f;
	isLooped = false;
	isFinished = false;
	isTree = false;

	_uint Flag = m_pPlayer->Get_TileInfoPacket().Range_FowardInfo.TileFlag;
	if ((TILE_FLAG::FLAG_TREE & Flag) != 0) {
		isTree = true;
	}

	HRESULT hr = E_FAIL;

	if (isTree)
	{
		auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
		 hr = Animator->Change_Animation("Tree_Shake.anim", true);

	}

	return hr;
}

void CPlayerState_ShakeTree::OnUpdate(_float dt)
{
	if (!isTree)
		return;

	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();

	
	if (InputDev->Key_Down(VK_SPACE)) {
		KeepTime += dt;
		if (isLooped == false) {
			
			if (KeepTime > 0.5f) {// /5초 이상 누르면 루프 진입
				isLooped = true;
				m_pPlayer->ActiveCollider_LeftHand(true, "KeepShake");
				m_pPlayer->ActiveCollider_RightHand(true, "KeepShake");
				Animator->Change_Animation("Tree_ShakeReadyKeep.anim", false);
			}
		}
	}
	if (isLooped)
	{
		if (InputDev->Key_Away(VK_SPACE)) {
			isFinished = true;
		}
	}
	if (!isLooped) {
		if (Animator->isOverAnimTiming(0.2f)) {
			m_pPlayer->ActiveCollider_LeftHand(true, "Shake");
			m_pPlayer->ActiveCollider_RightHand(true, "Shake");
		}
	}
}

HRESULT CPlayerState_ShakeTree::OnExit()
{
	m_pPlayer->ActiveCollider_LeftHand(false, "");
	m_pPlayer->ActiveCollider_RightHand(false, "");
	return S_OK;
}

CState* CPlayerState_ShakeTree::HandleTransition()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	
	if(!isTree)
		return m_pLayer->Get_State("Movement_Idle_State");

	if (!isLooped && Animator->isCurrentAnimEnd()) {
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	else if (isFinished && isLooped) {
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	return nullptr;
}

void CPlayerState_ShakeTree::Render_State()
{
}
_uint CPlayerState_ShakeTree::Get_InputMask() const
{
	return OnlyAction;
}
CPlayerState_ShakeTree* CPlayerState_ShakeTree::Create()
{
	return new CPlayerState_ShakeTree;
}

void CPlayerState_ShakeTree::Free()
{
	__super::Free();
}
