#include "Client_Defines.h"
#include "NpcState_Interact_ReceiveItem.h"

#include "EventSystem.h"
#include "ItemSpawner.h"
#include "NonPlayer.h"
#include "Animator3D.h"

#include "Item_Object.h"
#include "Player.h"

CNpcState_Interact_ReceiveItem::CNpcState_Interact_ReceiveItem()
{
	m_ItemData.eType = EVENT_TYPE::TransItem;
}

HRESULT CNpcState_Interact_ReceiveItem::OnEnter()
{
	auto Animator = m_pCharacter->Get_Component<CAnimator3D>();

	m_pCharacter->LookTo(
		m_pCharacter->Get_TracePack().pPlayer->Get_Component<CTransform>()->Get_Pos()
	);

	Animator->Change_Animation("Transfer_ReceiveForward.anim");
	m_ItemData.eType = EVENT_TYPE::TransItem_Response;
	m_ItemData.pObject = m_pCharacter->Get_ItemPack().pItem;
	m_ItemData.pSenderID = m_pCharacter->Get_TracePack().pPlayer->Get_ObjectID();

	m_eState = Receive;
	return S_OK;
}

void CNpcState_Interact_ReceiveItem::OnUpdate(_float dt)
{
	auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
	_float4x4 sockePtr = m_pCharacter->Get_SocketMatrix("Armature_Hand_R");
	XMStoreFloat4x4(&m_SocketMatrix, XMMatrixMultiply(XMLoadFloat4x4(&sockePtr), XMLoadFloat4x4(m_pCharacter->Get_Component<CTransform>()->Get_WorldMatrix_Ptr())));
	
	switch (m_eState)
	{
	case Client::CNpcState_Interact_ReceiveItem::Receive:
		if (Animator->isCurrentAnimEnd()) {
			m_pCharacter->Get_EventPack().eventSystem->OnBroadCast<BaseEvent>(m_ItemData);
			Animator->Change_Animation("Transfer_Putaway.anim");
			m_eState = Get;
		}
		break;
	case Client::CNpcState_Interact_ReceiveItem::Get:
		m_ItemData.pObject->Attach_Hand(&m_SocketMatrix);
		if (Animator->isCurrentAnimEnd()) {
			Animator->Change_Animation("Generic_Putaway.anim");
			m_ItemData.pObject->Remove_Item();
			m_eState = End;
		}
		break;
	case Client::CNpcState_Interact_ReceiveItem::End:
		if (m_pCharacter->Get_EventPack().hasNextSequence()) {
			m_pCharacter->Get_ItemPack().pItem = nullptr;
			m_pCharacter->Get_EventPack().reservedMsg.Type = "Talking";
			m_pCharacter->Get_EventPack().externalCondition = "AfterTrans";
		};
		break;
	default:
		break;
	}
}

HRESULT CNpcState_Interact_ReceiveItem::OnExit()
{
	
	return S_OK;
}

CState* CNpcState_Interact_ReceiveItem::HandleTransition()
{
	return nullptr;
}

void CNpcState_Interact_ReceiveItem::Render_State()
{
}

void CNpcState_Interact_ReceiveItem::OnClose(_bool isEnd)
{

}

CNpcState_Interact_ReceiveItem* CNpcState_Interact_ReceiveItem::Create()
{
	return  new CNpcState_Interact_ReceiveItem();
}

void CNpcState_Interact_ReceiveItem::Free()
{
}
