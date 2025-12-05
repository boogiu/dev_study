#include "Client_Defines.h"
#include "PlayerState_TransferGive.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "Level.h"
#include "EventSystem.h"
#include "Item_Object.h"

CPlayerState_TransferGive::CPlayerState_TransferGive()
{
}

HRESULT CPlayerState_TransferGive::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Release_AnimationBlend();
	m_pPlayer->Open_Inventory();
	return S_OK;
}

void CPlayerState_TransferGive::OnUpdate(_float dt)
{
	
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	switch (m_eState)
	{
	case Client::CPlayerState_TransferGive::IDLE:
		if (CGameObject* pObject = m_pPlayer->Get_InfoPack().pObjectOnLeftHand) {
			m_pPlayer->Close_Inventory();
			m_pPlayer->Play_Sound("Put_Out");
			m_ItemData.eType = EVENT_TYPE::TransItem;
			m_ItemData.pObject = dynamic_cast<CItem_Object*>(pObject);
			m_ItemData.Sender_InstanceID = m_pPlayer->Get_ObjectID();
			m_ItemData.Reciever_InstanceID = m_pPlayer->Get_InfoPack().pTalker->Get_ObjectID();
		Animator->Change_Animation("Transfer_PassForward.anim", false);
			m_eState = PullOut;
		}
		break;
	case Client::CPlayerState_TransferGive::PullOut:
		m_ItemData.pObject ->Attach_Hand(
			m_pPlayer->Get_InfoPack().pLeftHand->Get_Component<CTransform>()->Get_WorldMatrix_Ptr()
		);
		if (Animator->isOverAnimTiming(0.9)) {
			m_pPlayer->BroadCast_Event(m_ItemData);
			m_eState = Passing;
		}
		break;
	case Client::CPlayerState_TransferGive::Passing:
		if (nullptr ==  m_pPlayer->Get_InfoPack().pObjectOnLeftHand) {
		}
		break;
	case Client::CPlayerState_TransferGive::PutIn:
		break;
	case Client::CPlayerState_TransferGive::End:
		break;
	default:
		break;
	}
}

HRESULT CPlayerState_TransferGive::OnExit()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Restart_AnimationBlend();
	m_eState = IDLE;
	return S_OK;
}

CState* CPlayerState_TransferGive::HandleTransition()
{
	return nullptr;
}

void CPlayerState_TransferGive::Render_State()
{
}
_uint CPlayerState_TransferGive::Get_InputMask() const
{
	return 0;
}

CPlayerState_TransferGive* CPlayerState_TransferGive::Create()
{
	return new CPlayerState_TransferGive;
}

void CPlayerState_TransferGive::Free()
{
	__super::Free();
}
