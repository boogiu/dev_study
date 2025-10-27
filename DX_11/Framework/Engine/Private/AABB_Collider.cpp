#include "AABB_Collider.h"
#include "OBB_Collider.h"
#include "Sphere_Collider.h"
#include "Transform.h"
#include "GameObject.h"
#include "Build_Struct.h"
#include "GameInstance.h"
#include "ICollisionService.h"
#ifdef _DEBUG
#include "DebugDraw.h"
#endif

CAABB_Collider::CAABB_Collider()
{
}

CAABB_Collider::CAABB_Collider(const CAABB_Collider& rhs)
	:CCollider(rhs)
{
}

HRESULT CAABB_Collider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAABB_Collider::Initialize(COMPONENT_DESC* pArg)
{
	__super::Initialize(pArg);
	if (pArg != nullptr) {
		AABB_COLLIDER_DESC* pDesc = static_cast<AABB_COLLIDER_DESC*>(pArg);
		m_pOriginalDesc = new BoundingBox(pDesc->vCenter, pDesc->vSize);
		m_pDesc = new BoundingBox(*m_pOriginalDesc);
		CGameInstance::GetInstance()->Get_CollisionSystem()->ActiveCollider(this, m_SystemIndex);
	}
	return S_OK;
}

void CAABB_Collider::Update()
{
	if ((m_pDesc == nullptr) || (m_pOriginalDesc == nullptr)) return;

	m_prevCollider = m_CurrentCollider;
	m_CurrentCollider.clear();

	_float4x4 OwnerMatrix = m_pOwner->Get_Component<CTransform>()->Get_WorldMatrix();
	_matrix     TransformMatrix = XMLoadFloat4x4(&OwnerMatrix);

	TransformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(TransformMatrix.r[0]);
	TransformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(TransformMatrix.r[1]);
	TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(TransformMatrix.r[2]);

	m_pOriginalDesc->Transform(*m_pDesc, TransformMatrix);
	
}

void CAABB_Collider::Late_Update()
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

_bool CAABB_Collider::Intersect(CCollider* pOther)
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

	if (onCollision) {
		//pOther->m_CurrentCollider.emplace(this);
		m_CurrentCollider.emplace(pOther);
	}

	return onCollision;
}
void CAABB_Collider::Make_MinMaxCollider(MINMAX_BOX minMax)
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

	m_pOriginalDesc = new BoundingBox(vCenter,vSize);
	m_pDesc = new BoundingBox(*m_pOriginalDesc);
	CGameInstance::GetInstance()->Get_CollisionSystem()->ActiveCollider(this, m_SystemIndex);
}

#ifdef _DEBUG
void CAABB_Collider::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	DX::Draw(pBatch, *m_pDesc, vColor);
	return;
}
#endif

CAABB_Collider* CAABB_Collider::Create()
{
	CAABB_Collider* instance = new CAABB_Collider();
	if (FAILED(instance->Initialize_Prototype()))
	{
		Safe_Release(instance);
		MSG_BOX("CAABB_Collider Comp Failed To Create : CAABB_Collider");
	}
	return instance;
}

CComponent* CAABB_Collider::Clone()
{
	return new CAABB_Collider(*this);
}

void CAABB_Collider::Free()
{
	__super::Free();
	Safe_Delete(m_pDesc);
	Safe_Delete(m_pOriginalDesc);
}
