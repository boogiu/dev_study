#include "Client_Defines.h"
#include "FishSub_Tool.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "BoneFollower.h"
#include "Animator3D.h"
#include "AABB_Collider.h"

#include "GameInstance.h"
#include "Level.h"
#include "EventSystem.h"

#include "Fish_Object.h"

CFishSub_Tool::CFishSub_Tool()
{
}

CFishSub_Tool::CFishSub_Tool(const CFishSub_Tool& rhs)
	:CGameObject(rhs)
{
}

CFishSub_Tool::~CFishSub_Tool()
{
}

HRESULT CFishSub_Tool::Initialize_Prototype()
{

	__super::Initialize_Prototype();
	Add_Component<CSkeletalModel>();
	Add_Component<CMaterial>();
	Add_Component<CAABB_Collider>();
	return S_OK;
}

HRESULT CFishSub_Tool::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Get_Component<CSkeletalModel>()->Link_Model("GamePlay_Level", "Sub.model");
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "Sub.mat");
	Get_Component<CAABB_Collider>()->Set_ColliderActive(false);
	m_InstanceTag = "Sub";
	return S_OK;
}

void CFishSub_Tool::Awake()
{
	auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
	if (nowLevel) {
		auto eventSys = nowLevel->Get_LevelObject<CEventSystem>();
		eventSys->Add_Listner<CFishSub_Tool, BaseEvent>(this, &CFishSub_Tool::Get_Event);
	}
}

void CFishSub_Tool::Priority_Update(_float dt)
{

}

void CFishSub_Tool::Update(_float dt)
{
	switch (m_eState)
	{
	case Client::CFishSub_Tool::NonActive:
		break;
	case Client::CFishSub_Tool::Attach:
		FollowBone(dt);
		break;
	case Client::CFishSub_Tool::Throw:
		ThrowBey(dt);
		break;
	case Client::CFishSub_Tool::Flow:
		FlowBey(dt);
		break;
	case Client::CFishSub_Tool::ReAttached:
		ReturnToBone(dt);
		break;
	default:
		break;
	}
}

void CFishSub_Tool::Late_Update(_float dt)
{
}

void CFishSub_Tool::Render_GUI()
{
	__super::Render_GUI();

}


void CFishSub_Tool::Get_Event(const BaseEvent& event)
{
	if (m_eState == NonActive)
		return;

	if (event.eType == EVENT_TYPE::FishBeyThrow) {
		const auto& evt = static_cast<const POLE_THROW_EVENT&>(event);

		/*현재 붙어 있는 상태임*/
		if (m_eState == Attach && evt.bActiveAction == false) {
			m_eState = Throw;
			XMStoreFloat4(&m_vDirection, evt.vDirection);
			m_fThrowingTime = 0.f;

			m_vStartPos = Get_Position();

			_float speed = 25.f;
			_float3 dir = { m_vDirection.x, 0, m_vDirection.z };
			XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&dir)));
			m_vVelocity = { dir.x * speed, speed, dir.z * speed,1.f };
		}

		/*현재 붙어 있지 않은 상태이고 액션 키 눌렸음*/
		else if (evt.bActiveAction) {
			if (m_pTarget) {
				_bool CanHit = m_pTarget->Hit();
				if (CanHit) {
					auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
					if (nowLevel) {
						auto eventSys = nowLevel->Get_LevelObject<CEventSystem>();
						eventSys->OnBroadCast<BaseEvent>(POLE_BITE_RESULT{ EVENT_TYPE::FishBeyResult, POLE_BITE_RESULT::BITE ,nullptr });
						m_eState = Hit;
					}
				}
				else {
					m_eState = Attach;
					Get_Component<CAABB_Collider>()->Set_ColliderActive(false);
					m_pTarget->Missed();
					m_pTarget = nullptr;
				}
			}
			else {
				m_eState = Attach;
				Get_Component<CAABB_Collider>()->Set_ColliderActive(false);
				m_pTarget = nullptr;
			}
		}
	};

	if (m_pTarget) {
		if (event.eType == EVENT_TYPE::FishBeyReceive) {
		const auto& evt = static_cast<const POLE_BEY_RECIEVE&>(event);
		m_eState = ReAttached;
		m_vLeftHand = evt.pDestPos;
		m_pTarget->Catch();
	}
	}
}

void CFishSub_Tool::OnCollisionEnter(COLLISION_CONTEXT context)
{
	if (!m_pTarget && context.Owner->Has_Tag("Fish")) {
		m_pTarget = dynamic_cast<CFish_Object*>(context.Owner);
		m_pTarget->Set_TargetBey(this);
		Get_Component<CAABB_Collider>()->Set_ColliderActive(false);
	}
}

void CFishSub_Tool::OnCollisionStay(COLLISION_CONTEXT context)
{
}

void CFishSub_Tool::OnCollisionExit(COLLISION_CONTEXT context)
{
}

void CFishSub_Tool::Sync_Bont_To_Rod(_float4x4* pOwnerMatrix)
{
	if (pOwnerMatrix) {
		m_eState = Attach;
		m_pOwnerMatrix = pOwnerMatrix;
	}
}


void CFishSub_Tool::FollowBone(_float dt)
{
	if (!m_pOwnerMatrix)
		return;
	_matrix ownerTransform = XMLoadFloat4x4(m_pOwner->Get_Component<CTransform>()->Get_WorldMatrix_Ptr());
	_matrix DstMat = XMLoadFloat4x4(m_pOwnerMatrix) * ownerTransform;

	_vector myPos = m_pTransform->Get_Pos();
	_vector S, R, T;
	XMMatrixDecompose(&S, &R, &T, DstMat);

	//_vector movePos = XMVectorLerp(myPos, T, dt);
	_vector movePos = T - myPos;
	m_pTransform->Translate(movePos * dt * 8);
}

void CFishSub_Tool::ThrowBey(_float dt)
{
	m_fThrowingTime += dt;

	_float3 pos;
	pos.x = m_vStartPos.x + m_vVelocity.x * m_fThrowingTime;
	pos.z = m_vStartPos.z + m_vVelocity.z * m_fThrowingTime;
	pos.y = m_vStartPos.y + m_vVelocity.y * m_fThrowingTime + (-9.8f * m_fThrowingTime * m_fThrowingTime * 2.f);

	if (pos.y <= -1.5f)
	{
		pos.y = -1.5f;
		m_fThrowingTime = 0.f;
		m_fFloatingTime = 0.f;
		Get_Component<CAABB_Collider>()->Set_ColliderActive(true);
		Get_Component<CAABB_Collider>()->Make_MinMaxCollider({ { -20,-2,-20 }, { 20,2,20} });
		Get_Component<CAABB_Collider>()->Set_ContextEvent("Finding_Fish");
		m_eState = Flow;
	}

	m_pTransform->Set_Pos(pos);
}

void CFishSub_Tool::FlowBey(_float dt)
{
	m_fFloatingTime += dt * 3;
	_float y = sinf(m_fFloatingTime) * 3;
	m_pTransform->Translate({ 0, y * dt, 0 });
}

void CFishSub_Tool::ReturnToBone(_float dt)
{
	_vector dst = XMLoadFloat4(m_vLeftHand) - m_pTransform->Get_Pos();;

	m_pTransform->Translate(dst * dt);
}

void CFishSub_Tool::Missed()
{
	m_pTarget == nullptr;
}

CFishSub_Tool* CFishSub_Tool::Create()
{
	CFishSub_Tool* instance = new CFishSub_Tool();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CFishSub_Tool");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CFishSub_Tool::Clone(INIT_DESC* pArg)
{
	CFishSub_Tool* instance = new CFishSub_Tool(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CFishSub_Tool");
		Safe_Release(instance);
	}

	return instance;
}

void CFishSub_Tool::Free()
{
	__super::Free();
}