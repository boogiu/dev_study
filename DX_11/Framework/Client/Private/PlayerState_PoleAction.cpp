#include "Client_Defines.h"
#include "PlayerState_PoleAction.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "Level.h"
#include "EventSystem.h"
#include "Fish_Object.h"

CPlayerState_PoleAction::CPlayerState_PoleAction()
{
}

HRESULT CPlayerState_PoleAction::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Stop_AnimationBlend();

	auto TilePack = m_pPlayer->Get_TileInfoPacket();
	_uint Flag = TilePack.Range_FowardInfo.TileFlag;

	HRESULT hr;

	if ((TILE_FLAG::FLAG_RIVER & Flag) != 0) {
		hr = Animator->Change_Animation("ToolPole_Swing.anim");
		m_pPlayer->Change_Tool_Anim("ToolPoleAnim_Swing.anim");
	}
	else {
		hr = Animator->Change_Animation("ToolPole_Air.anim", true);
		m_pPlayer->Change_Tool_Anim("ToolPoleAnim_Air.anim");
		m_eState = AIR;
	}

	return S_OK;
}

void CPlayerState_PoleAction::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	m_LeftHand = m_pPlayer->Get_InfoPack().pLeftHand->Get_Position();
	if (m_eState== PULLBACK && m_pPlayer->Get_InfoPack().isFishBitted) {
		m_eState = HIT;
		Animator->Change_Animation("ToolPole_Hit.anim");
		Animator->Change_Animation("ToolPoleAnim_Hit.anim");
	}
	if (m_eState == CATCH && m_pPlayer->Get_InfoPack().pObjectOnLeftHand) {
		m_eState = GET;
		Animator->Change_Animation("ToolPole_CatchKeep.anim");
		Animator->Change_Animation("ToolPoleAnim_CatchKee.anim");
		isCatch = true;
	}

	switch (m_eState)
	{
	case Client::CPlayerState_PoleAction::BEGIN:
		if (Animator->isOverAnimTiming(0.4f)) {
			m_pPlayer->BroadCast_Event(POLE_THROW_EVENT{ EVENT_TYPE::FishBeyThrow, m_pPlayer->Get_Component<CTransform>()->Dir(STATE::LOOK),false });
			m_eState = THROW;
		}
		break;

	case Client::CPlayerState_PoleAction::AIR:
		if (Animator->isCurrentAnimEnd()) {
			m_eState = END;
		}
		break;

	case Client::CPlayerState_PoleAction::THROW:
		if (Animator->isCurrentAnimEnd()) {
			Animator->Change_Animation("ToolPole_Aim.anim");
			m_pPlayer->Change_Tool_Anim("ToolPoleAnim_Aim.anim");
			m_eState = AIM;
		}
		break;	

	case Client::CPlayerState_PoleAction::AIM:
		if (m_pPlayer->Get_ControlPack().MsgAction) {
			Animator->Change_Animation("ToolPole_Putback.anim");
			m_pPlayer->Change_Tool_Anim("ToolPoleAnim_Putback.anim");

			m_pPlayer->BroadCast_Event(
				POLE_THROW_EVENT{ EVENT_TYPE::FishBeyThrow, {},true });
				m_eState = PULLBACK;
		}
		break;

	case Client::CPlayerState_PoleAction::HIT:
		m_HitTime += dt;

		if (m_HitTime > 2.5f) {
			m_eState = CATCH;
			m_pPlayer->BroadCast_Event(POLE_BEY_RECIEVE{ EVENT_TYPE::FishBeyReceive, &m_LeftHand });
			Animator->Change_Animation("ToolPole_Catch.anim");
			Animator->Change_Animation("ToolPoleAnim_Catch.anim"); 
			m_HitTime = 0.f;
		}
		break;

	case Client::CPlayerState_PoleAction::PULLBACK:
		if (Animator->isCurrentAnimEnd()) {
			m_eState = END;
		}
		break;

	case Client::CPlayerState_PoleAction::CATCH:
		break;
	case Client::CPlayerState_PoleAction::GET:
		m_HitTime += dt;
		 
		if (m_HitTime > .5f) {
			m_pPlayer->Change_Tool_Anim("ToolPoleAnim_CompleteKeep.anim");
			m_eState = END;
		}

		dynamic_cast<CFish_Object*>(m_pPlayer->Get_InfoPack().pObjectOnLeftHand)->Get();
		m_pPlayer->Get_InfoPack()
			.pObjectOnLeftHand->Get_Component<CTransform>()->TranslateMatrix(
				XMLoadFloat4x4(m_pPlayer->Get_InfoPack().pLeftHand->Get_Component<CTransform>()->Get_WorldMatrix_Ptr())
			);
		break;
	case Client::CPlayerState_PoleAction::END:
		break;
	default:
		break;
	}
}

HRESULT CPlayerState_PoleAction::OnExit()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Restart_AnimationBlend();
	m_pPlayer->Change_Tool_Anim("ToolPoleAnim_APose.anim");
	m_eState = BEGIN;
	m_HitTime = 0.f;
	isCatch = true;
	return S_OK;
}

CState* CPlayerState_PoleAction::HandleTransition()
{
	if (m_eState == END&&!isCatch) {
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	if (m_eState == END && isCatch) {
		return m_pLayer->Get_State("Action_Get_State");
	}
	return nullptr;
}

void CPlayerState_PoleAction::Render_State()
{
}

_uint CPlayerState_PoleAction::Get_InputMask() const
{
	return static_cast<_uint>(InputMask::ACTION);
}

CPlayerState_PoleAction* CPlayerState_PoleAction::Create()
{
	return new CPlayerState_PoleAction;
}
void CPlayerState_PoleAction::Free()
{
	__super::Free();
}