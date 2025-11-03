#include "Client_Defines.h"
#include "PlayerState_TransTool.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "ObjectContainer.h"
#include "PlayerPart_Hand.h"

CPlayerState_TransTool::CPlayerState_TransTool()
{
}

HRESULT CPlayerState_TransTool::OnEnter()
{

	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Release_AnimationBlend();

	auto Pack = m_pPlayer->Get_ItemPacket();

	if (Pack.DstItem.TypeTag == itemType::None) {
		Animator->Change_Animation("Base_EquipOff.anim");
	}
	else {
		Animator->Change_Animation("Base_EquipOn.anim");
	}
	m_bTransed = false;

	return S_OK;
}

void CPlayerState_TransTool::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	auto& Pack = m_pPlayer->Get_ItemPacket();

	if (!m_bTransed) {
		if (Animator->isOverAnimTiming(0.5f)) {
			m_pPlayer->Set_CurItemData(Pack.DstItem);
			m_bTransed = true;
		}
	}
	
}

HRESULT CPlayerState_TransTool::OnExit()
{
	return S_OK;
}

CState* CPlayerState_TransTool::HandleTransition()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	if (Animator->isCurrentAnimEnd()) {
		return m_pLayer->Get_State("Movement_Idle_State");
	}

	return nullptr;
}

void CPlayerState_TransTool::Render_State()
{
}

_uint CPlayerState_TransTool::Get_InputMask() const
{
	return 0;
}

CPlayerState_TransTool* CPlayerState_TransTool::Create()
{
	return new CPlayerState_TransTool;
}

void CPlayerState_TransTool::Free()
{
}
