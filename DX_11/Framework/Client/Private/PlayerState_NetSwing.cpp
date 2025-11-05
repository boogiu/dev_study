#include "Client_Defines.h"
#include "PlayerState_NetSwing.h"
#include "PlayerStateMachine.h"
#include "Player.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "Builder.h"

CPlayerState_NetSwing::CPlayerState_NetSwing()
{
}

HRESULT CPlayerState_NetSwing::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Stop_AnimationBlend();

	auto TilePack = m_pPlayer->Get_TileInfoPacket();
	_uint Flag = TilePack.Range_FowardInfo.TileFlag;
	HRESULT hr;

	if ((TILE_FLAG::FLAG_INSECT & Flag) != 0) {
		hr = Animator->Change_Animation("ToolNet_Swing.anim");
	}
	else {
		hr = Animator->Change_Animation("ToolNet_SwingStop_Ground.anim", true);
	}
	m_pPlayer->ActiveCollider_Tool(true, "NetSwing");


	return hr;
}

void CPlayerState_NetSwing::OnUpdate(_float dt)
{

}

HRESULT CPlayerState_NetSwing::OnExit()
{
	m_pPlayer->ActiveCollider_Tool(false, "");

	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Restart_AnimationBlend();
	isCathced = false;
	return S_OK;
}

CState* CPlayerState_NetSwing::HandleTransition()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	CPlayer::ControlPacket control = m_pPlayer->Get_ControlPack();
	if (isCathced) {
		return m_pLayer->Get_State("Action_Get_State");
	}
	else {
		if (Animator->isOverAnimTiming(0.95f)) {
			return m_pLayer->Get_State("Movement_Idle_State");
		}
	}

	return nullptr;
}

void CPlayerState_NetSwing::Render_State()
{
}

_uint CPlayerState_NetSwing::Get_InputMask() const
{
	return static_cast<_uint>(InputMask::MOVE);
}

void CPlayerState_NetSwing::OnCollisionEnter(COLLISION_CONTEXT context)
{
	if (context.Owner->Has_Tag("Insect")) {
		isCathced = true;
		if (context.Owner->Get_Component<CModel>()) {
			context.Owner->Get_Component<CModel>()->Set_CompActive(false);
			m_pPlayer->Get_InfoPack().m_pObjectOnLeftHand = context.Owner;
		}
	}
}
void CPlayerState_NetSwing::OnCollisionStay(COLLISION_CONTEXT context)
{
	
}

CPlayerState_NetSwing* CPlayerState_NetSwing::Create()
{
	return new CPlayerState_NetSwing;
}

void CPlayerState_NetSwing::Free()
{
	__super::Free();
}
