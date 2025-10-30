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
	auto TilePack = m_pPlayer->Get_TileInfoPacket();
	_uint Flag = TilePack.infos[Get_Index(NEIGHBOR_INDEX::UP)].TileFlag;
	_uint ItemFlag = Flag | TilePack.infos[Get_Index(NEIGHBOR_INDEX::CENTER)].TileFlag;
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();
	CPlayer::ControlPacket control = m_pPlayer->Get_ControlPack();

	if (control.MsgToolUse) {
		if ((TILE_FLAG::FLAG_ONITEM & ItemFlag) != 0) {
			m_pStateMachine->Request_ChangeState(STATE_LAYER::ACTION, "Movement_PickUp_State");
		}
		//나중에 여기에 뭔가를 심을건지 판별하는 로직 추가 필요함.
		else {
			m_pStateMachine->Request_ChangeState(STATE_LAYER::ACTION, "Action_Dig_State");
			return m_pLayer->Get_State("Tool_Base_State");
		}
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
