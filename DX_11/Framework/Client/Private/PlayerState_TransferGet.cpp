#include "Client_Defines.h"
#include "PlayerState_TransferGet.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "Level.h"
#include "EventSystem.h"
#include "Item_Object.h"

CPlayerState_TransferGet::CPlayerState_TransferGet()
{
}

HRESULT CPlayerState_TransferGet::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Release_AnimationBlend();
	auto TrasData = m_pPlayer->Get_InfoPack().m_nowTrans;
	m_data = {EVENT_TYPE::TransItem, TrasData.pObject,TrasData.pSenderID };

	m_pPlayer->Get_InfoPack().m_nowTrans = {};
	if (m_data.pObject) {
		Animator->Change_Animation("Transfer_ReceiveForward.anim");
		m_pPlayer->Camera_Zoom_In(m_pPlayer->Get_InfoPack().pTalker);
		m_eState = Transfered;
	}
	return S_OK;
}

void CPlayerState_TransferGet::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	switch (m_eState)
	{
	case Client::CPlayerState_TransferGet::Transfered:
		if (m_data.pObject) {
			Animator->Change_Animation("Transfer_ReceiveBack.anim");
			m_eState = Passing;
		}
		break;
	case Client::CPlayerState_TransferGet::Passing:
		if (Animator->isOverAnimTiming(0.8)) {
			Animator->Change_Animation("Transfer_Putaway.anim");
			m_data.pObject->Attach_Hand(
				m_pPlayer->Get_InfoPack().pLeftHand->Get_Component<CTransform>()->Get_WorldMatrix_Ptr()
			);
			m_eState = PutIn;
		}
		break;
	case Client::CPlayerState_TransferGet::PutIn:
		m_fOutime += dt;
		if (m_fOutime > 1.f) {
			if (Animator->isCurrentAnimEnd()) {
				Animator->Change_Animation("Generic_Putaway.anim");
				m_pPlayer->Add_ITEM(m_data.pObject->Get_ItemData());
				m_data.pObject->Remove_Item();
				m_eState = End;
			}
		}
		break;
	default:
		break;
	}
}

HRESULT CPlayerState_TransferGet::OnExit()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Restart_AnimationBlend();

	RESPONSE_TRANS_ITEM data = {};
	data.eType = EVENT_TYPE::TransItem_Response;
	data.pSenderID = m_data.pSenderID;
	data.pObject = data.pObject;
	CGameInstance::GetInstance()->Get_CurrentLevel()->Get_LevelObject<CEventSystem>()->OnBroadCast<BaseEvent>(
		data
	);

	m_pPlayer->Camera_Zoom_Out();
	m_pPlayer->Get_InfoPack().m_nowTrans = {};
	m_data = {};
	m_fOutime = 0;
	return S_OK;
}

CState* CPlayerState_TransferGet::HandleTransition()
{
	if (m_eState == End)
		return m_pLayer->Get_State("Movement_Idle_State");
	return nullptr;
}

void CPlayerState_TransferGet::Render_State()
{
}
_uint CPlayerState_TransferGet::Get_InputMask() const
{
	return 0;
}

CPlayerState_TransferGet* CPlayerState_TransferGet::Create()
{
	return new CPlayerState_TransferGet;
}

void CPlayerState_TransferGet::Free()
{
	__super::Free();
}
