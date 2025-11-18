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
	return S_OK;
}

HRESULT CFish_Object::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	FishSpawnDesc* pDesc = static_cast<FishSpawnDesc*>(pArg);

	HRESULT hr = Add_Component<CSkeletalModel>()->Link_Model("GamePlay_Level", pDesc->FishDataDesc.sizeTag+".model");
	hr = Add_Component<CMaterial>()->Link_Material("GamePlay_Level", pDesc->FishDataDesc.sizeTag + ".mat");
	Add_Component<CAnimator3D>()->LinkAnimate_Model("GamePlay_Level", pDesc->FishDataDesc.sizeTag + ".model");

	m_ItemDesc = pDesc->itemDataDesc;
	m_InstanceTag = "Fish";
	m_InstanceName = pDesc->FishDataDesc.FishFileName;
	m_FishDesc = pDesc->FishDataDesc;

	Get_Component<CAnimator3D>()->Add_AnimClips("GamePlay_Level", "Swim.anim", pDesc->FishDataDesc.sizeTag, true);
	Get_Component<CAnimator3D>()->Change_Animation("Swim.anim");
	Get_Component<CAABB_Collider>()->Make_MinMaxCollider({ {-3,-5,-3},{3,5,3 } });

	m_fDetectRange = 30.f;
	return S_OK;
}

void CFish_Object::Priority_Update(_float dt)
{
	m_fElapseTime += dt;

}

void CFish_Object::Update(_float dt)
{
	Get_Component<CAnimator3D>()->Update_Animation(dt);

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
	m_pTransform->Translate({ m_vAxis.x * dt*3,0,m_vAxis.y * dt * 3 });
	Check_Rotation(dt);
}

void CFish_Object::Late_Update(_float dt)
{
}

void CFish_Object::Render_GUI()
{
	__super::Render_GUI();
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
		_float4 nextPos = { nowPos.x + NewAxis.x ,nowPos.y,nowPos.z+ NewAxis.y,1.f };


		_bool inWater = TileSys->Check_TileFlagByPosition(nextPos,static_cast<_uint>(TILE_FLAG::FLAG_RIVER));

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
	m_fDestRadian= atan2(m_vAxis.x, m_vAxis.y);
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

CGameObject* CFish_Object::Clone(INIT_DESC * pArg)
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
