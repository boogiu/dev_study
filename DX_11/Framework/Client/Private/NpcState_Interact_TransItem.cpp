#include "Client_Defines.h"
#include "NpcState_Interact_TransItem.h"

#include "EventSystem.h"
#include "ItemSpawner.h"
#include "NonPlayer.h"
#include "Animator3D.h"

#include "Item_Object.h"
CNpcState_Interact_TransItem::CNpcState_Interact_TransItem()
{

}

HRESULT CNpcState_Interact_TransItem::OnEnter()
{
	auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
	Animator->Change_Animation("Generic_PullOut.anim", false);
	m_eState = PullOut;

	//		TALKING_EVENT event{ m_pCharacter,nullptr, "Player" };
	//		m_pCharacter->Get_EventPack().eventSystem->OnBroadCast(event);
	//		
	//		auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
	//		Animator->Change_Animation("Base_Wait.anim", true);
	//		
	//		m_pCharacter->Open_Dialogue("TalkingMsg", &desc);
	//		m_pCharacter->LookTo(m_pCharacter->Get_TracePack().pPlayer->Get_Component<CTransform>()->Get_Pos());

	return S_OK;
}

void CNpcState_Interact_TransItem::OnUpdate(_float dt)
{
	auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
	_float4x4 sockePtr = Animator->Get_BoneMatrix(m_pCharacter->Get_ReservedPack().Socketbone);
	
	XMStoreFloat4x4(&m_SocketMatrix, XMMatrixMultiply(XMLoadFloat4x4(&sockePtr), XMLoadFloat4x4(m_pCharacter->Get_Component<CTransform>()->Get_WorldMatrix_Ptr())));
	switch (m_eState)
	{
	case Client::CNpcState_Interact_TransItem::PullOut:
		if (Animator->isOverAnimTiming(0.8))
		{
			string itemTag = m_pCharacter->Get_ReservedPack().reservedAction.Param1;
			CItem_Object* pObj = m_pCharacter->Spawn_Item(itemTag);
			//본의 파이널 매트릭스에다가 내 월드 매트릭스를 곱해줘야 하지
			pObj->Attach_Hand(&m_SocketMatrix);
			m_eState = ItemHandle;
		}
		break;
	case Client::CNpcState_Interact_TransItem::ItemHandle:
		if (Animator->isCurrentAnimEnd())
		{
			m_eState = Transferred;
		}
		break;
	case Client::CNpcState_Interact_TransItem::Transferred:

		break;
	case Client::CNpcState_Interact_TransItem::End:
		break;
	default:
		break;
	}
}

HRESULT CNpcState_Interact_TransItem::OnExit()
{
	return S_OK;
}

CState* CNpcState_Interact_TransItem::HandleTransition()
{
	return nullptr;
}

void CNpcState_Interact_TransItem::Render_State()
{
}

TalkingMsgDesc CNpcState_Interact_TransItem::Make_EvtDesc()
{
	TalkingMsgDesc desc = {};
	desc.OpenSize = { 800,160 };
	desc.OpenSpeed = 8.f;
	desc.SpeakerID = m_pCharacter->Get_NpcData().NpcID;
	desc.startSequence = m_pCharacter->Get_EventPack().Ready_SequenceID;
	desc.Speaker = m_pCharacter;

	desc.OnClose = [this](_bool isEnd) {OnClose(isEnd); };
	return desc;
}

void CNpcState_Interact_TransItem::OnClose(_bool isEnd)
{

}

CNpcState_Interact_TransItem* CNpcState_Interact_TransItem::Create()
{
	return  new CNpcState_Interact_TransItem();
}

void CNpcState_Interact_TransItem::Free()
{
}
