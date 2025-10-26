#include "Client_Defines.h"
#include "PlayerState_TransItem.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "ObjectContainer.h"
#include "PlayerPart_Hand.h"

CPlayerState_TransItem::CPlayerState_TransItem()
{
}

void CPlayerState_TransItem::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	ITEM_DATA_DESC dstDesc = m_pPlayer->Get_DstItemData();
	if (dstDesc.eType == ITEM_TYPE::NONE) {
		Animator->Chane_Animation("Base_EquipOff.anim");
	}
	else {
		Animator->Chane_Animation("Base_EquipOn.anim");
	}
	m_bTransed = false;
}

void CPlayerState_TransItem::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (!m_bTransed) {
		if (Animator->isOverAnimTiming(0.5f)) {
			m_pPlayer->Set_CurItemData(m_pPlayer->Get_DstItemData());
			m_bTransed = true;
		}
	}
	
}

void CPlayerState_TransItem::OnExit()
{
}

CState* CPlayerState_TransItem::HandleTransition()
{
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (Animator->isCurrentAnimEnd()) {
		return m_pHFSM->Get_State("Idle_Base_State");
	}

	return nullptr;
}

void CPlayerState_TransItem::Render_State()
{
}

CPlayerState_TransItem* CPlayerState_TransItem::Create()
{
	return new CPlayerState_TransItem;
}

void CPlayerState_TransItem::Free()
{
}
