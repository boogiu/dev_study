#include "Client_Defines.h"
#include "NpcState_Interact_TransItem.h"

#include "EventSystem.h"
#include "ItemSpawner.h"
#include "NonPlayer.h"
#include "Animator3D.h"

#include "Item_Object.h"
#include "Player.h"

CNpcState_Interact_TransItem::CNpcState_Interact_TransItem()
{
	m_ItemData.eType = EVENT_TYPE::TransItem;
}

HRESULT CNpcState_Interact_TransItem::OnEnter()
{
	auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
	Animator->Change_Animation("Transfer_PassForward.anim", false);
	m_eState = PullOut;

	m_pCharacter->Get_ActionPack().Begin("Transfer_Item");
	return S_OK;
}

void CNpcState_Interact_TransItem::OnUpdate(_float dt)
{
	auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
	_float4x4 sockePtr = m_pCharacter->Get_SocketMatrix("Armature_Hand_R");

	XMStoreFloat4x4(&m_SocketMatrix, XMMatrixMultiply(XMLoadFloat4x4(&sockePtr), XMLoadFloat4x4(m_pCharacter->Get_Component<CTransform>()->Get_WorldMatrix_Ptr())));
	_vector socketPos, s, r;
	XMMatrixDecompose(&s, &r, &socketPos, XMLoadFloat4x4(&m_SocketMatrix));
	_float3 socketPosition;
	m_pCharacter->Get_TracePack().pPlayer->Get_InfoPack().pTalker = m_pCharacter;

	XMStoreFloat3(&socketPosition, socketPos);
 	switch (m_eState)
	{
	case Client::CNpcState_Interact_TransItem::PullOut:
		if (Animator->isOverAnimTiming(0.4))
		{
			/*어디서 지금 건넬 아이템 정보를 가져올 것인가.*/
			string itemTag = m_pCharacter->Get_EventPack().ConsumePostAction().Param1;
			CItem_Object* pObj = m_pCharacter->Spawn_Item(itemTag, socketPosition);

			//본의 파이널 매트릭스에다가 내 월드 매트릭스를 곱해줘야 하지
			pObj->Attach_Hand(&m_SocketMatrix);
			m_ItemData.pObject = pObj;
			m_ItemData.Sender_InstanceID = m_pCharacter->Get_ObjectID();
			m_ItemData.Reciever_InstanceID = m_pCharacter->Get_TracePack().pPlayer->Get_ObjectID();
			m_eState = ItemHandle;
			m_pCharacter->Get_ActionPack().NextPhase();
		}
		break;

	case Client::CNpcState_Interact_TransItem::ItemHandle:
		if (Animator->isCurrentAnimEnd())
		{
			m_pCharacter->Get_EventPack().eventSystem->OnBroadCast<BaseEvent>(m_ItemData);
			m_eState = Transferred;
		}
		break;
	case Client::CNpcState_Interact_TransItem::Transferred:
		if (m_pCharacter->Get_ActionPack().Is("Transfer_Item", ActionPhase::End)) {
			m_eState = End;
		}
		break;
	case Client::CNpcState_Interact_TransItem::End:
		Animator->Change_Animation("Base_Wait.anim", false);
		m_pCharacter->Get_ActionPack().Reset();

		if (m_pCharacter->Get_EventPack().hasNextSequence()) {
			m_pCharacter->Get_EventPack().reservedMsg.Type = "Talking";
			m_pCharacter->Get_EventPack().externalCondition= "AfterTrans";
		};

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
