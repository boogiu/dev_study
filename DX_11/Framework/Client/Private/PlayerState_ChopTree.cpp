#include "Client_Defines.h"
#include "PlayerState_ChopTree.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_ChopTree::CPlayerState_ChopTree()
{
}

HRESULT CPlayerState_ChopTree::OnEnter()
{
	/*블렌드 멈추고, 이 상태가 지속되는 동안에는 계속해서 여기*/
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Stop_AnimationBlend();

	auto TilePack = m_pPlayer->Get_TileInfoPacket();
	_uint Flag = TilePack.Range_FowardInfo.TileFlag;

	m_bOnTree = false;

	HRESULT hr = E_FAIL;

	if ((TILE_FLAG::FLAG_TREE& Flag)==0) {
		if ((TILE_FLAG::FLAG_BLOCKED & Flag) != 0) {
			 hr = Animator->Change_Animation("ToolAxe_Repelled.anim", true);
		}
		else {
			hr = Animator->Change_Animation("ToolAxe_Air.anim", true);
		}
	}
	else if ((TILE_FLAG::FLAG_TREE & Flag) != 0) {
		hr = Animator->Change_Animation("ToolAxe_Hit.anim", true);
		m_bOnTree = true;
	}

	return hr;
}

void CPlayerState_ChopTree::OnUpdate(_float dt)
{

	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();

	if (m_bOnTree) {
		if (Animator->isOverAnimTiming(0.2f)) {
			m_pPlayer->ActiveCollider_Tool(true);
		}
	}

}

HRESULT CPlayerState_ChopTree::OnExit()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Restart_AnimationBlend();
	m_pPlayer->ActiveCollider_Tool(false);

	return S_OK;
}

CState* CPlayerState_ChopTree::HandleTransition()
{
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (Animator->isCurrentAnimEnd()) {
		return m_pLayer->Get_State("Movement_Idle_State");
	}

	return nullptr; 
}

void CPlayerState_ChopTree::Render_State()
{
}

_uint CPlayerState_ChopTree::Get_InputMask() const
{
	return FlagForAction;
}

CPlayerState_ChopTree* CPlayerState_ChopTree::Create()
{
	return new CPlayerState_ChopTree;
}

void CPlayerState_ChopTree::Free()
{
}
