#include "Client_Defines.h"
#include "PlayerState_Get.h"

#include "PlayerStateMachine.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"

CPlayerState_Get::CPlayerState_Get()
{
}
HRESULT CPlayerState_Get::OnEnter()
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Stop_AnimationBlend();
	m_pPlayer->Adjust_To_WorldFoward();
	itemType nowType = m_pPlayer->Get_ItemPacket().CurItem.TypeTag;
	switch (nowType)
	{
	case itemType::None:
	case itemType::Axe:
	case itemType::Scoop:
		Animator->Change_Animation("Generic_Get.anim", true);
		m_ePhase = Phase::Idle;
		break;
	case itemType::Net:
		Animator->Change_Animation("ToolNet_Get.anim", true);
		m_ePhase = Phase::Priority;
		break;
	default:
		break;
	}

	return S_OK;
}

void CPlayerState_Get::OnUpdate(_float dt)
{
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	auto InfoPack = m_pPlayer->Get_InfoPack();
	if (CGameObject* pObject = InfoPack.m_pObjectOnLeftHand) {
		if (m_ePhase == Idle)
			pObject->Get_Component<CModel>()->Set_CompActive(true);
		_float4 pos = InfoPack.m_pLeftHand->Get_Position();
		pos.z += 1;
		pObject->Get_Component<CTransform>()->TranslateMatrix(XMLoadFloat4x4(InfoPack.m_pLeftHand->Get_WorldMatrix()));
	}
	switch (m_ePhase)
	{
	case Client::CPlayerState_Get::Priority:
		if (Animator->isOverAnimTiming(0.95)) {
			m_pPlayer->Camera_Zoom_In();
			Animator->Change_Animation("Generic_Get.anim", true);
			m_ePhase = Idle;
		}
		break;

	case Client::CPlayerState_Get::Idle:
		if (Animator->isOverAnimTiming(0.95)) {
			Animator->Change_Animation("Generic_GetKeep.anim", true);
			m_ePhase = Keep;
		}
		break;

	case Client::CPlayerState_Get::Keep:
		if (!m_bMsgComplete) {
			EventMsgDesc desc = {};
			desc.OpenSize = { 600,150 };
			desc.OpenSpeed = 8.f;
			desc.textSequence = { L"응? 이건...", L"고추 잠자리를 잡았다!\n저녁 노을 같은 붉은 색!" };
			desc.OnClose = [&]() {
				auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
				Animator->Change_Animation("Generic_Putaway.anim", true);
				m_pPlayer->Camera_Zoom_Out();
				m_ePhase = PutIn;
				};
			m_pPlayer->Open_EventMsg(&desc);
			m_bMsgComplete = true;
		}

		break;
	case Client::CPlayerState_Get::PutIn: {

		if (Animator->isCurrentAnimEnd()) {
			m_ePhase = End;
		}
	}
										break;
	case Client::CPlayerState_Get::End:
		break;
	default:
		break;
	}

}

HRESULT CPlayerState_Get::OnExit()
{
	m_ePhase = Phase::Priority;
	auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
	Animator->Restart_AnimationBlend();
	CGameInstance::GetInstance()->Get_ObjectMgr()->Remove_Object(m_pPlayer->Get_InfoPack().m_pObjectOnLeftHand);
	m_pPlayer->Get_InfoPack().m_pObjectOnLeftHand = nullptr;
	m_bMsgComplete = false;
	return S_OK;
}

CState* CPlayerState_Get::HandleTransition()
{

	if (m_ePhase == End)
		return m_pLayer->Get_State("Movement_Idle_State");

	return nullptr;
}

_uint CPlayerState_Get::Get_InputMask() const
{
	return 0;
}

void CPlayerState_Get::Render_State()
{
}

CPlayerState_Get* CPlayerState_Get::Create()
{
	return new CPlayerState_Get;
}

void CPlayerState_Get::Free()
{
	__super::Free();
}