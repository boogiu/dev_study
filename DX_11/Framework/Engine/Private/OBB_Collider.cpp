#include "OBB_Collider.h"
#include "Sphere_Collider.h"
#include "AABB_Collider.h"

#include "Transform.h"
#include "GameObject.h"
#include "Build_Struct.h"
#include "GameInstance.h"
#include "ICollisionService.h"
#ifdef _DEBUG
#include "DebugDraw.h"
#endif

COBB_Collider::COBB_Collider()
{
}

COBB_Collider::COBB_Collider(const COBB_Collider& rhs)
	:CCollider(rhs)
{
}

HRESULT COBB_Collider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT COBB_Collider::Initialize(COMPONENT_DESC* pArg)
{
	__super::Initialize(pArg);
	if (pArg != nullptr) {
		OBB_COLLIDER_DESC* pDesc = static_cast<OBB_COLLIDER_DESC*>(pArg);
		_float4     vQuaternion = {};
		XMStoreFloat4(&vQuaternion,
			XMQuaternionRotationRollPitchYaw(pDesc->vEularRadians.x, pDesc->vEularRadians.y, pDesc->vEularRadians.z));
		m_pOriginalDesc = new BoundingOrientedBox(pDesc->vCenter, pDesc->vSize, vQuaternion);
		m_pDesc = new BoundingOrientedBox(*m_pOriginalDesc);
		CGameInstance::GetInstance()->Get_CollisionSystem()->ActiveCollider(this, m_SystemIndex);
	}
	return S_OK;
}

void COBB_Collider::Update()
{
	if ((m_pDesc == nullptr) || (m_pOriginalDesc == nullptr)) return;
	_float4x4 OwnerMatrix = m_pOwner->Get_Component<CTransform>()->Get_WorldMatrix();
	_matrix     TransformMatrix = XMLoadFloat4x4(&OwnerMatrix);
	m_pOriginalDesc->Transform(*m_pDesc, TransformMatrix);
}

void COBB_Collider::Late_Update()
{
	if ((m_pDesc == nullptr) || (m_pOriginalDesc == nullptr)) return;
	for (auto& other : m_CurrentCollider) {
		if (m_prevCollider.find(other) == m_prevCollider.end()) {
			//지금 있고 이전에 없음
			m_pOwner->OnCollisionEnter(other->Get_Context());
		}
		else {
			//지금 있고 이전에 있음
			m_pOwner->OnCollisionStay(other->Get_Context());
		}
	}
	for (auto& other : m_prevCollider) {
		if (m_CurrentCollider.find(other) == m_CurrentCollider.end()) {
			//이전에 있고 지금 없음
			m_pOwner->OnCollisionExit(other->Get_Context());
		}
	}
}

_bool COBB_Collider::Intersect(CCollider* pOther)
{
	_bool       onCollision = { false };

	switch (pOther->Get_ColliderType())
	{
	case Engine::COLLIDER_TYPE::AABB:
		onCollision = m_pDesc->Intersects(*(static_cast<CAABB_Collider*>(pOther)->Get_Desc()));
		break;
	case Engine::COLLIDER_TYPE::OBB:
		onCollision = m_pDesc->Intersects(*(static_cast<COBB_Collider*>(pOther)->Get_Desc()));
		break;
	case Engine::COLLIDER_TYPE::SPHERE:
		onCollision = m_pDesc->Intersects(*(static_cast<CSphere_Collider*>(pOther)->Get_Desc()));
		break;
	default:
		break;
	}

	return onCollision;
}
void COBB_Collider::Make_MinMaxCollider(MINMAX_BOX minMax)
{
	if (m_pOriginalDesc) {
		Safe_Delete(m_pOriginalDesc);
		Safe_Delete(m_pDesc);
	}

	_float3 vCenter = {
	(minMax.vMax.x + minMax.vMin.x)*0.5f,
	(minMax.vMax.y + minMax.vMin.y) * 0.5f,
	(minMax.vMax.z + minMax.vMin.z) * 0.5f
	};
	_float3 vSize = {
	(minMax.vMax.x - minMax.vMin.x) * 0.5f,
	(minMax.vMax.y - minMax.vMin.y) * 0.5f,
	(minMax.vMax.z - minMax.vMin.z) * 0.5f
	};

	_vector qRotation = m_pOwner->Get_Component<CTransform>()->Get_QuaternionRotate();
	_float4     vQuaternion = {};
	XMStoreFloat4(&vQuaternion, qRotation);

	m_pOriginalDesc = new BoundingOrientedBox(vCenter, vSize, vQuaternion);
	m_pDesc = new BoundingOrientedBox(*m_pOriginalDesc);

	CGameInstance::GetInstance()->Get_CollisionSystem()->ActiveCollider(this, m_SystemIndex);
}

#ifdef _DEBUG
void COBB_Collider::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	DX::Draw(pBatch, *m_pDesc, vColor);
	return;
}
#endif

COBB_Collider* COBB_Collider::Create()
{
	COBB_Collider* instance = new COBB_Collider();
	if (FAILED(instance->Initialize_Prototype()))
	{
		Safe_Release(instance);
		MSG_BOX("COBB_Collider Comp Failed To Create : COBB_Collider");
	}
	return instance;
}

CComponent* COBB_Collider::Clone()
{
	return new COBB_Collider(*this);
}

void COBB_Collider::Free()
{
	__super::Free();
	Safe_Delete(m_pDesc);
	Safe_Delete(m_pOriginalDesc);
}
