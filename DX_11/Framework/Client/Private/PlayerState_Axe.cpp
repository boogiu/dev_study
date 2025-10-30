#include "Client_Defines.h"
#include"PlayerState_Axe.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_Axe::CPlayerState_Axe()
{
}

void CPlayerState_Axe::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Set_AnimationBlend("ToolAxe_APose.anim", { 19,20,21,22,23,24,25,26,27,28,29,30,31 });
}

void CPlayerState_Axe::OnUpdate(_float dt)
{
}

void CPlayerState_Axe::OnExit()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
}

CState* CPlayerState_Axe::HandleTransition()
{
	auto TilePack = m_pPlayer->Get_TileInfoPacket();
	_uint Flag = TilePack.infos[Get_Index(NEIGHBOR_INDEX::UP)].TileFlag;
	_uint ItemFlag = Flag | TilePack.infos[Get_Index(NEIGHBOR_INDEX::CENTER)].TileFlag;
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();
	CPlayer::ControlPacket control = m_pPlayer->Get_ControlPack();

	if (control.MsgToolUse) {
		if ((TILE_FLAG::FLAG_ONITEM & ItemFlag) != 0) {
			m_pStateMachine->Request_ChangeState(STATE_LAYER::ACTION, "Movement_PickUp_State");
		}
		else {
			m_pStateMachine->Request_ChangeState(STATE_LAYER::ACTION, "Action_TreeChop_State");
			return m_pLayer->Get_State("Tool_Base_State");
		}
	}
	return nullptr;
}

void CPlayerState_Axe::Render_State()
{
}

_uint CPlayerState_Axe::Get_InputMask() const
{
	return ToolAndAction;
}

CPlayerState_Axe* CPlayerState_Axe::Create()
{
	return new CPlayerState_Axe;
}

void CPlayerState_Axe::Free()
{
	__super::Free();
}
