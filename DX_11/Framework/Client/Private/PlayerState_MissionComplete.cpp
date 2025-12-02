#include "Client_Defines.h"
#include "PlayerState_MissionComplete.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "Level.h"
#include "EventSystem.h"
#include "Item_Object.h"
#include "UI_Responcer.h"

CPlayerState_MissionComplete::CPlayerState_MissionComplete()
{
}

HRESULT CPlayerState_MissionComplete::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Release_AnimationBlend();
	m_pPlayer->Adjust_To_WorldFoward();
	m_pPlayer->Camera_Zoom_In();
	if (m_pPlayer->Get_ItemPacket().CurItem.TypeTag != itemType::None) {
		m_pPlayer->Request_State(STATE_LAYER::TOOL, "Tool_Release_State");
		m_eState = Priority;
	}
	else {
		Animator->Change_Animation("Event_LoanComplete.anim");
		m_eState = Start;
	}
	m_pPlayer->Play_Sound("Complete");
	return S_OK;
}

void CPlayerState_MissionComplete::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	switch (m_eState)
	{
	case Client::CPlayerState_MissionComplete::Priority:
		if (Animator->isCurrentAnimEnd()) {
			Animator->Change_Animation("Event_LoanComplete.anim");
			m_eState = Start;
		}
		break;
	case Client::CPlayerState_MissionComplete::Start:
		if (Animator->isCurrentAnimEnd()) {
			Animator->Change_Animation("Event_LoanCompleteKeep.anim");
			EventMsgDesc desc = Make_Sequence();
			m_pPlayer->Open_EventMsg(&desc);
			m_eState = Keep;
		}
		break;
	case Client::CPlayerState_MissionComplete::Keep:
		break;
	case Client::CPlayerState_MissionComplete::End:
		break; 
	default:
		break;
	}
}

HRESULT CPlayerState_MissionComplete::OnExit()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	
	return S_OK;
}

CState* CPlayerState_MissionComplete::HandleTransition()
{
	if (m_eState == End)
		return m_pLayer->Get_State("Movement_Idle_State");

	return nullptr;
}

void CPlayerState_MissionComplete::Render_State()
{
}
_uint CPlayerState_MissionComplete::Get_InputMask() const
{
	return 0;
}

void CPlayerState_MissionComplete::Complete()
{
	auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
	auto UI_Response = nowLevel->Get_LevelObject<CUI_Responcer>();
	UI_Response->Active_UI("Screen_FX", nullptr);
	m_pPlayer->BroadCast_Event(ENDING_EVENT{ EVENT_TYPE::Ending, true });
	m_eState = End;
	m_pPlayer->Get_ControlPack().MsgForceBlock = true;
	m_pPlayer->Camera_Far_Out();
}

EventMsgDesc CPlayerState_MissionComplete::Make_Sequence()
{
	EventMsgDesc desc = {};
	desc.OpenSize = { 600,150 };
	desc.OpenSpeed = 8.f;
	desc.textSequence = { L"드디어...!", L"KK군의 노래를 들을 수 있어!"};
	desc.OnClose = [this]() {Complete(); };
	return desc;
}

CPlayerState_MissionComplete* CPlayerState_MissionComplete::Create()
{
	return new CPlayerState_MissionComplete;
}

void CPlayerState_MissionComplete::Free()
{
	__super::Free();
}
