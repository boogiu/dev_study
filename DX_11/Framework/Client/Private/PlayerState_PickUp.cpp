#include "Client_Defines.h"
#include "PlayerState_PickUp.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_PickUp::CPlayerState_PickUp()
{
}

HRESULT CPlayerState_PickUp::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	HRESULT hr =E_FAIL;
	_uint Flag =m_pPlayer->Get_TileInfoPacket().Range_FowardInfo.TileFlag;

	if ((TILE_FLAG::FLAG_ONITEM & Flag) != 0) {
		Animator->Stop_AnimationBlend();
		Item_Found = true;
		m_pPlayer->Adjust_To_Foward();
		 hr = Animator->ForceChange_Animation("Pickup.anim", false);
	}
	else {
		return E_FAIL;
	}

	return hr;
}

void CPlayerState_PickUp::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	if (Item_Found) {
		if (Animator->isOverAnimTiming(0.3)) {
			m_pPlayer->ActiveCollider_LeftHand(true, "Pick_Up");
			if (!m_SoundComplete) {
				m_pPlayer->Play_Sound("Pick_Up");
				m_SoundComplete = true;
			}
		}
	}
}

HRESULT CPlayerState_PickUp::OnExit()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Restart_AnimationBlend(); 
	Item_Found = false;
	m_SoundComplete = false;
	return S_OK;
}

CState* CPlayerState_PickUp::HandleTransition()
{
	if (Item_Found == false) {
		return m_pLayer->Get_State("Movement_Idle_State");
	}
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	if (Animator->isCurrentAnimEnd()) {
		m_pPlayer->ActiveCollider_LeftHand(false, "");
		return m_pLayer->Get_State("Movement_Idle_State");
	}

	return nullptr;
}

void CPlayerState_PickUp::Render_State()
{
}

_uint CPlayerState_PickUp::Get_InputMask() const
{
	return OnlyAction;
}

CPlayerState_PickUp* CPlayerState_PickUp::Create()
{
	return new CPlayerState_PickUp;
}

void CPlayerState_PickUp::Free()
{
}
