#include "Client_Defines.h"
#include "PlayerState_CraftAction.h"
#include "Animator3D.h"
#include "Item_Object.h"
/*Action_Craft_State*/
CPlayerState_CraftAction::CPlayerState_CraftAction()
{
}

HRESULT CPlayerState_CraftAction::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Release_AnimationBlend();
	Animator->Change_Animation("Etc_DiyCreating.anim");
	m_eState = Start;
	m_pPlayer->Adjust_To_Foward();

	auto transform = 	m_pPlayer->Get_Component<CTransform>();

	_float4 forward = m_pPlayer->Get_InfoPack().pEncounter->Get_Position();
	forward.y += 10.f;
	
	m_pPlayer->Request_Effect("Effect_Craft", { forward,forward });
	return S_OK;
}

void CPlayerState_CraftAction::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

	auto pObj = m_pPlayer->Get_InfoPack().pObjectOnLeftHand;
	CItem_Object* pItem = dynamic_cast<CItem_Object*>(pObj);

	switch (m_eState)
	{
	case Client::CPlayerState_CraftAction::Start:
		m_ElapsedTime += dt;
		pItem->Attach_Hand(m_pPlayer->Get_InfoPack().pLeftHand->Get_Component<CTransform>()->Get_WorldMatrix_Ptr());
		if (m_ElapsedTime > 3.5f) {
			m_eState = Finished;
			Animator->Change_Animation("Etc_DiyFinish.anim");
			m_ElapsedTime = 0;
		}
		break;
	case Client::CPlayerState_CraftAction::Finished:
		m_pPlayer->Adjust_To_WorldFoward();
		pItem->Get_Component<CModel>()->Set_CompActive(true);
		if (Animator->isCurrentAnimEnd()) {
			Animator->Change_Animation("Generic_Get.anim", true);
			m_pPlayer->Camera_Zoom_In();
			m_eState = Get;
		}
		break;
	case Client::CPlayerState_CraftAction::Get:
		if (Animator->isOverAnimTiming(0.95)) {
			Animator->Change_Animation("Generic_GetKeep.anim");
			m_eState = GetKeep;
		}
		break;
	case Client::CPlayerState_CraftAction::GetKeep:
		m_ElapsedTime += dt;
		if (m_ElapsedTime > 2.5f) {
			m_eState = Putin;
			Animator->Change_Animation("Generic_Putaway.anim");
			pItem->Remove_Item();
			m_pPlayer->Add_ITEM(pItem->Get_ItemData());
			m_pPlayer->Get_InfoPack().pObjectOnLeftHand = nullptr;
		}
		break;
	case Client::CPlayerState_CraftAction::Putin:
		if (Animator->isCurrentAnimEnd()) {
			m_pPlayer->Camera_Restore();
			m_eState = End;
		}
		break;
	case Client::CPlayerState_CraftAction::End:
		break;
	default:
		break;
	}
}

HRESULT CPlayerState_CraftAction::OnExit()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Restart_AnimationBlend();
	return S_OK;
}

CState* CPlayerState_CraftAction::HandleTransition()
{
	if (m_eState == End)
		return m_pLayer->Get_State("Movement_Idle_State");
	return nullptr;
}

void CPlayerState_CraftAction::Render_State()
{
}

_uint CPlayerState_CraftAction::Get_InputMask() const
{
	return 0;
}

CPlayerState_CraftAction* CPlayerState_CraftAction::Create()
{
	return new CPlayerState_CraftAction;
}
void CPlayerState_CraftAction::Free()
{
	__super::Free();
}
