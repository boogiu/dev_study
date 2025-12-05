#include "Client_Defines.h"
#include "PlayerState_Get.h"

#include "PlayerStateMachine.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"

#include "Level.h"
#include "InsectSpawner.h"
#include "Insect_Object.h"
#include "Fish_Object.h"
#include "FishSpawner.h"
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
	case itemType::FishingRod:
		Animator->Change_Animation("ToolPole_Get.anim", true);
		m_ePhase = Phase::Priority;
		m_bFish = true;
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
		if (m_ePhase == Idle|| m_ePhase == Priority)
		{
			if(m_ePhase == Idle)
				pObject->Get_Component<CModel>()->Set_CompActive(true);

			_matrix boneWorld =
				XMLoadFloat4x4(Animator->Get_BoneMatrixPtr("Armature_Hand_L")) *
				XMLoadFloat4x4(m_pPlayer->Get_Component<CTransform>()->Get_WorldMatrix_Ptr());

			_vector scale, rot, trans;
			XMMatrixDecompose(&scale, &rot, &trans, boneWorld);
			trans += _vector{ 0,0,2,0 };
			_float angle = XMConvertToRadians(-90.f);
			_vector axis = m_bFish? XMVectorSet(0.f, 1.f, 0.f, 0.f) : XMVectorSet(1.f, 0.f, 0.f, 0.f);

			_vector newRot = XMQuaternionRotationAxis(axis, angle);
			_matrix mScale = XMMatrixScalingFromVector(scale);
			_matrix mRot = XMMatrixRotationQuaternion(newRot);
			_matrix mTrans = XMMatrixTranslationFromVector(trans);
			_matrix world = mScale * mRot * mTrans;

			auto pTr = pObject->Get_Component<CTransform>();
			pTr->TranslateMatrix(world);
		}

	}
	switch (m_ePhase)
	{
	case Client::CPlayerState_Get::Priority:
		if (Animator->isOverAnimTiming(0.95)) {
			m_pPlayer->Camera_Zoom_In();
			Animator->Change_Animation("Generic_Get.anim", true);
			m_pPlayer->Play_Sound("Get");
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
		m_pPlayer->Get_InfoPack().pObjectOnLeftHand->Get_Component<CTransform>()->AddScale({ -dt,-dt,-dt });
		if (Animator->isOverAnimTiming(0.5)) {
			m_pPlayer->Get_InfoPack().pObjectOnLeftHand->Get_Component<CModel>()->Set_CompActive(false);
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
	m_bFish = false;
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
	else if (pObject->Has_Tag("Fish")) {
		auto spawner = m_pPlayer->Get_NowLevel()->Get_LevelObject<CFishSpawner>();
		FISH_DATA_DESC data = spawner->Get_Data(pObject->Get_InstanceName());
		wstring CapturedMsg = data.FishName + L"를 잡았다!";
		desc.textSequence = { L"응? 이건...", CapturedMsg + L"\n" + data.Comment };
	}

	desc.OnClose = [&]() {
		auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
		Animator->Change_Animation("Generic_Putaway.anim", true);
		m_pPlayer->Camera_Restore();
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
	else if (pObject->Has_Tag("Fish")) {
		m_pPlayer->Add_ITEM(dynamic_cast<CFish_Object*>(pObject)->Get_ItemData());
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