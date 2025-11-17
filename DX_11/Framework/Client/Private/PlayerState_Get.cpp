#include "Client_Defines.h"
#include "PlayerState_Get.h"

#include "PlayerStateMachine.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"

#include "Level.h"
#include "InsectSpawner.h"
#include "Insect_Object.h"

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

	if (CGameObject* pObject = InfoPack.pObjectOnLeftHand) {
		if (m_ePhase == Idle)
			pObject->Get_Component<CModel>()->Set_CompActive(true);
		_float4 pos = InfoPack.pLeftHand->Get_Position();
		pObject->Get_Component<CTransform>()->TranslateMatrix(XMLoadFloat4x4(InfoPack.pLeftHand->Get_WorldMatrix()));
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
			EventMsgDesc desc = Make_Sequence(m_pPlayer->Get_InfoPack().pObjectOnLeftHand);

			m_pPlayer->Open_EventMsg(&desc);
			m_bMsgComplete = true;
		}

		break;
	case Client::CPlayerState_Get::PutIn: {

		if (Animator->isCurrentAnimEnd()) {
			Add_Inventory(m_pPlayer->Get_InfoPack().pObjectOnLeftHand);
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
	CGameInstance::GetInstance()->Get_ObjectMgr()->Remove_Object(m_pPlayer->Get_InfoPack().pObjectOnLeftHand);
	m_pPlayer->Get_InfoPack().pObjectOnLeftHand = nullptr;
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

EventMsgDesc CPlayerState_Get::Make_Sequence(CGameObject* pObject)
{
	EventMsgDesc desc = {};
	desc.OpenSize = { 600,150 };
	desc.OpenSpeed = 8.f;

	if (pObject->Has_Tag("Item")) {

	}
	else if (pObject->Has_Tag("Insect")) {
		auto spawner = m_pPlayer->Get_NowLevel()->Get_LevelObject<CInsectSpawner>();
		INSECT_DATA_DESC data = spawner->Get_Data(pObject->Get_InstanceName());
		wstring CapturedMsg = data.InsectName + L"를 잡았다!";
		desc.textSequence = { L"응? 이건...", CapturedMsg + L"\n" + data.Comment };
	}

	desc.OnClose = [&]() {
		auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
		Animator->Change_Animation("Generic_Putaway.anim", true);
		m_pPlayer->Camera_Zoom_Out();
		m_ePhase = PutIn;
		};

	return desc;
}

void CPlayerState_Get::Add_Inventory(CGameObject* pObject)
{
	if (pObject->Has_Tag("Item")) {

	}
	else if (pObject->Has_Tag("Insect")) {
		m_pPlayer->Add_ITEM(dynamic_cast<CInsect_Object*>(pObject)->Get_ItemData());
	}

}

CPlayerState_Get* CPlayerState_Get::Create()
{
	return new CPlayerState_Get;
}

void CPlayerState_Get::Free()
{
	__super::Free();
}