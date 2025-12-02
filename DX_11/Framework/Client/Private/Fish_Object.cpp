#include "Client_Defines.h"
#include "Fish_Object.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "Animator3D.h"

#include "AABB_Collider.h"
#include "GameInstance.h"
#include "TileSystem.h"
#include "Helper_Func.h"

#include "FishSub_Tool.h"

#include "Level.h"
#include "EffectSpawner.h"
#include "AudioSource.h"

CFish_Object::CFish_Object()
{
}

CFish_Object::CFish_Object(const CFish_Object& rhs)
	:CGameObject(rhs)
{
}

HRESULT CFish_Object::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CAABB_Collider>();
	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "Fish_Captured.wav", "Captured", false, SOUND_GROUP::SFX, 1.f);
	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "Fish_Pick.wav", "Pick", false, SOUND_GROUP::SFX, 1.f);
	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "Fish_Struggle_M.wav", "Struggle", true, SOUND_GROUP::SFX, 1.f);
	return S_OK;
}

HRESULT CFish_Object::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	FishSpawnDesc* pDesc = static_cast<FishSpawnDesc*>(pArg);

	HRESULT hr = Add_Component<CSkeletalModel>()->Link_Model("GamePlay_Level", pDesc->FishDataDesc.sizeTag + ".model");
	hr = Add_Component<CMaterial>()->Link_Material("GamePlay_Level", pDesc->FishDataDesc.sizeTag + ".mat");
	Add_Component<CAnimator3D>()->LinkAnimate_Model("GamePlay_Level", pDesc->FishDataDesc.sizeTag + ".model");

	m_ItemDesc = pDesc->itemDataDesc;
	m_InstanceTag = "Fish";
	m_InstanceName = pDesc->FishDataDesc.FishFileName;
	m_FishDesc = pDesc->FishDataDesc;

	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Swim.anim", pDesc->FishDataDesc.sizeTag, true);
	Get_Component<CAnimator3D>()->Change_Animation("Swim.anim");
	Get_Component<CAABB_Collider>()->Make_MinMaxCollider({ {-2,-2,-2},{2,5,2,} });

	auto& Instances = Get_Component<CMaterial>()->Get_Material_Instance();
	for (auto instance : Instances)
	{
		instance->Set_Param("fFishAlpha", { &m_fAlpha, "float", sizeof(float) });
		//fFishAlpha
	}
	return S_OK;
}

void CFish_Object::Priority_Update(_float dt)
{
	m_fElapseTime += dt;
}

void CFish_Object::Update(_float dt)
{
	Get_Component<CAnimator3D>()->Update_Animation(dt);

	switch (m_eState)
	{
	case Client::CFish_Object::IDLE:
		Move_IDLE(dt);
		break;
	case Client::CFish_Object::FIND_ENTRANCE:
		Move_FIND(dt);
		break;
	case Client::CFish_Object::FIND_BACK:
		Move_BACK(dt);
		break;
	case Client::CFish_Object::HIT:
		Move_HIT(dt);
		break;
	case Client::CFish_Object::BITE:
		Move_BITE(dt);
		break;
	case Client::CFish_Object::CATCHED:
		Move_CATCHED(dt);
		break;
	case Client::CFish_Object::DISAPPEAR:
		Move_DISAPPEAR(dt);
		break;
	case Client::CFish_Object::GETTED:
		break;
	default:
		break;
	}
}

void CFish_Object::Late_Update(_float dt)
{
}

void CFish_Object::Render_GUI()
{
	static _bool m;
	m = m_eState == HIT;
	ImGui::Checkbox("Hit", &(m));
	__super::Render_GUI();
}

void CFish_Object::Set_TargetBey(CGameObject* pTarget)
{
	m_pTarget = dynamic_cast<CFishSub_Tool*>(pTarget);
	if (!m_pTarget)
		return;
	m_eState = FIND_ENTRANCE;
	m_BiteCount = Helper::Get_Random_Int(2, 4);
	m_fDetectTime = 0.f;
}

_bool CFish_Object::Hit()
{
	if (m_eState == HIT) {
		m_eState = BITE;
		Get_Component<CAudioSource>()->Play("Struggle");
	}

	return m_eState == BITE;
}

void CFish_Object::Missed()
{
	m_eState = DISAPPEAR;
}

void CFish_Object::Catch()
{
	m_eState = CATCHED;
	Get_Component<CAudioSource>()->Set_SlotPuase("Struggle",true);
	HRESULT hr = Add_Component<CSkeletalModel>()->Link_Model("GamePlay_Level", m_FishDesc.modelName);
	hr = Add_Component<CMaterial>()->Link_Material("GamePlay_Level", m_FishDesc.materialName);
	Add_Component<CAnimator3D>()->LinkAnimate_Model("GamePlay_Level", m_FishDesc.modelName);
}

void CFish_Object::Get()
{
	m_eState = GETTED;
	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Get.anim", m_FishDesc.FishFileName, true);
	Get_Component<CAnimator3D>()->Change_Animation("GamePlay_Level", "Get.anim");
}

void CFish_Object::OnCollisionEnter(COLLISION_CONTEXT context)
{

}

void CFish_Object::OnCollisionStay(COLLISION_CONTEXT context)
{
}

void CFish_Object::OnCollisionExit(COLLISION_CONTEXT context)
{
}

void CFish_Object::Move_IDLE(_float dt)
{
	if (m_fElapseTime > 5.f) {
		Check_Axis();
	}

	auto TileSys = CGameInstance::GetInstance()->Get_TileSystem();
	_float4 nowPos = Get_Position();
	_float4 nextPos = { nowPos.x + m_vAxis.x ,nowPos.y,nowPos.z + m_vAxis.y,1.f };
	_bool inWater = TileSys->Check_TileFlagByPosition(nextPos, static_cast<_uint>(TILE_FLAG::FLAG_RIVER));

	if (!inWater) {
		Check_Axis();
	}

	m_pTransform->Translate({ m_vAxis.x * dt * 3,0,m_vAxis.y * dt * 3 });
	Check_Rotation(dt);
}

void CFish_Object::Move_FIND(_float dt)
{
	if (!m_pTarget)
		return;

	_vector TargetPos = m_pTarget->Get_Component<CTransform>()->Get_Pos();
	_vector MyPos = m_pTransform->Get_Pos();


	_vector vDistance = TargetPos - MyPos;
	XMVectorSetY(vDistance, 0); /*Y이도 지우기*/

	auto TileSys = CGameInstance::GetInstance()->Get_TileSystem();
	_vector Move = vDistance * dt * 3;
	_float4 NextPos = {};
	XMStoreFloat4(&NextPos, Move + m_pTransform->Get_Pos());
	_bool inWater = TileSys->Check_TileFlagByPosition(NextPos, static_cast<_uint>(TILE_FLAG::FLAG_RIVER));

	if (inWater)
		m_pTransform->Translate(Move);

	_vector direction = XMVector4Normalize(vDistance);
	m_vAxis = { XMVectorGetX(direction), XMVectorGetZ(direction) };

	Check_Rotation(dt);

	if (XMVectorGetX(XMVector3Length(vDistance)) < 1.f) {
		m_eState = FIND_BACK;
		m_NowBiteCount++;


		if (m_NowBiteCount > m_BiteCount) {
			m_eState = HIT;
			m_fDetectTime = 0.f;
			auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
			auto EffectSys = nowLevel->Get_LevelObject<CEffectSpawner>();
			EffectSys->Request_Effect("Effect_WaterSplash", { Get_Position(), Get_Position() });
			Get_Component<CAudioSource>()->Play("Captured");
		}
		else {
			m_pTarget->Pong(Get_Position());
			Get_Component<CAudioSource>()->Play("Pick");
		}
	}
}


void CFish_Object::Move_BACK(_float dt)
{
	if (!m_pTarget)
		return;

	auto TileSys = CGameInstance::GetInstance()->Get_TileSystem();
	_vector inverseDir = _vector{ m_vAxis.x,0,m_vAxis.y,1 } *-1;
	_vector Move = inverseDir * dt * 3;
	_float4 NextPos = {};
	XMStoreFloat4(&NextPos, Move + m_pTransform->Get_Pos());
	_bool inWater = TileSys->Check_TileFlagByPosition(NextPos, static_cast<_uint>(TILE_FLAG::FLAG_RIVER));

	if (inWater)
		m_pTransform->Translate(Move);

	_vector TargetPos = m_pTarget->Get_Component<CTransform>()->Get_Pos();
	_vector MyPos = m_pTransform->Get_Pos();

	_vector vDistance = TargetPos - MyPos;
	XMVectorSetY(vDistance, 0); /*Y이도 지우기*/

	if (XMVectorGetX(XMVector3Length(vDistance)) > 8.f || !inWater) {
		m_eState = FIND_ENTRANCE;
	}
}

void CFish_Object::Move_HIT(_float dt)
{
	m_fDetectTime += dt;
	if (m_fDetectTime > .8f) {
		m_eState = DISAPPEAR;
	}
}

void CFish_Object::Move_BITE(_float dt)
{
	m_fDetectTime += dt * 15;
	m_fBiteTime += dt;
	m_fEffectTime += dt;

	_float4 pos = m_pTarget->Get_Position();

	_vector targetPos = m_pTarget->Get_Component<CTransform>()->Get_Pos();
	_vector myPos = m_pTransform->Get_Pos();
	_vector dir = targetPos - myPos;

	m_vAxis.x = XMVectorGetX(dir);
	m_vAxis.y = XMVectorGetZ(dir);

	_float4 CirclePos = {
		pos.x + cosf(m_fDetectTime) * 2.5f,
		pos.y,
		pos.z + sinf(m_fDetectTime) * 2.5f,
		1.f
	};


	m_pTransform->Set_Pos(CirclePos);
	m_pTransform->LookAt(targetPos);

	if (m_fEffectTime > 0.15f) {
		auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
		auto EffectSys = nowLevel->Get_LevelObject<CEffectSpawner>();
		EffectSys->Request_Effect("Effect_WaterWaveSplash", { Get_Position(), Get_Position() });
		m_fEffectTime = 0.f;
	}
}

void CFish_Object::Move_CATCHED(_float dt)
{
	_vector targetPos = m_pTarget->Get_Component<CTransform>()->Get_Pos();
	_vector myPos = m_pTransform->Get_Pos();

	_vector pos = XMVectorLerp(myPos, targetPos, dt * 4);
	m_pTransform->Set_PosVector(pos);
	m_pTransform->LookAt(pos);
}


void CFish_Object::Move_DISAPPEAR(_float dt)
{
	_vector targetPos = m_pTarget->Get_Component<CTransform>()->Get_Pos();
	_vector myPos = m_pTransform->Get_Pos();
	_vector dir = (targetPos - myPos) * -1.f;
	_vector direction = XMVector4Normalize(dir);

	m_vAxis.x = XMVectorGetX(direction);
	m_vAxis.y = XMVectorGetZ(direction);

	Check_Rotation(dt);

	m_pTransform->Translate(direction * dt * 3);
	m_fAlpha -= dt * 0.5f;

	if (m_fAlpha < 0.1f) {
		CGameInstance::GetInstance()->Get_ObjectMgr()->Remove_Object(this);
		m_pTarget->Missed();
	}
}

void CFish_Object::Check_Axis()
{
	auto TileSys = CGameInstance::GetInstance()->Get_TileSystem();

	_int TryCount = {};

	while (TryCount < 10)
	{
		_float RandomX = Helper::Get_Random_Float(-2.f, 2.f);
		_float RandomZ = Helper::Get_Random_Float(-2.f, 2.f);

		_float2 NewAxis = m_vAxis;
		NewAxis.x += RandomX;
		NewAxis.y += RandomZ;

		XMStoreFloat2(&NewAxis, XMVector2Normalize(XMLoadFloat2(&NewAxis)));

		_float4 nowPos = Get_Position();
		_float4 nextPos = { nowPos.x + NewAxis.x*8 ,nowPos.y,nowPos.z + NewAxis.y * 8,1.f };

		_bool inWater = TileSys->Check_TileFlagByPosition(nextPos, static_cast<_uint>(TILE_FLAG::FLAG_RIVER));

		if (inWater) {
			m_vAxis = NewAxis;
			break;
		}

		TryCount++;
	}

	m_fElapseTime = 0;
}

void CFish_Object::Check_Rotation(_float dt)
{
	m_fDestRadian = atan2(m_vAxis.x, m_vAxis.y);
	_float DeltaRadian = m_fDestRadian - m_fCurrentRadian;

	while (DeltaRadian > XM_PI)  DeltaRadian -= XM_2PI;
	while (DeltaRadian < -XM_PI)  DeltaRadian += XM_2PI;

	_float RotSpeed = dt * 5.f;

	if (fabs(DeltaRadian) > .3f)
		m_fCurrentRadian += (DeltaRadian > 0 ? RotSpeed : -RotSpeed);
	else
		m_fCurrentRadian = m_fDestRadian;
	m_pTransform->Override_Rotation({ 0,1,0,0 }, m_fCurrentRadian);
}

_bool CFish_Object::InWater()
{
	return _bool();
}

CFish_Object* CFish_Object::Create()
{
	CFish_Object* instance = new CFish_Object();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CFish_Object");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CFish_Object::Clone(INIT_DESC* pArg)
{
	CFish_Object* instance = new CFish_Object(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CFish_Object");
		Safe_Release(instance);
	}

	return instance;
}

void CFish_Object::Free()
{
	__super::Free();
}
