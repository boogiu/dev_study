#include "Transform.h"

CTransform::CTransform()
{
}

CTransform::CTransform(const CTransform& rhs)
	:CComponent(rhs)
{
}

HRESULT CTransform::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTransform::Initialize(COMPONENT_DESC* pArg)
{
	if (pArg == nullptr)
		return S_OK;

	TRANSFORM_DESC* desc = static_cast<TRANSFORM_DESC*>(pArg);
	
	_fvector position = XMVectorSetW(XMLoadFloat3(&desc->vInitialPosition), 1.f);
	XMStoreFloat4(&m_vPosition,position);
	_fvector scale = XMVectorSetW(XMLoadFloat3(&desc->vInitialScale), 0.f);
	XMStoreFloat4(&m_vScale, scale);

	/*오일러로 받아서 -> 쿼터니언으로*/
	_fvector euler = XMVectorSetW(XMLoadFloat3(&desc->vInitialEulerVector), 0.f);
	_fvector  quaternion= XMQuaternionRotationRollPitchYawFromVector(euler);
	XMStoreFloat4(&m_qRotation, quaternion);

	m_bDirty = true;
	return S_OK;
}

void CTransform::Translate(_fvector momentVector)
{
	_vector vPos = XMLoadFloat4(&m_vPosition);
	vPos += momentVector;

	XMStoreFloat4(&m_vPosition, vPos);
	m_bDirty = true;
}


void CTransform::Rotation(_fvector eulerVector)
{
	_fvector addQuaternion = XMQuaternionRotationRollPitchYawFromVector(eulerVector);
	_fvector myQuaternion = XMLoadFloat4(&m_qRotation);
	_fvector newQuaternion = XMQuaternionMultiply(addQuaternion, myQuaternion);
	XMQuaternionNormalize(newQuaternion);
	XMStoreFloat4(&m_qRotation, newQuaternion);
	m_bDirty = true;
}

void CTransform::Set_Pos(const _float3& position)
{
	_fvector newPosition = XMVectorSetW(XMLoadFloat3(&position), 1.f);
	XMStoreFloat4(&m_vPosition, newPosition);
	m_bDirty = true;
}

void CTransform::Set_Rotate(const _float3& _eular)
{
	_fvector euler = XMVectorSetW(XMLoadFloat3(&_eular), 0.f);
	_fvector  quaternion = XMQuaternionRotationRollPitchYawFromVector(euler);
	XMStoreFloat4(&m_qRotation, quaternion);
	m_bDirty = true;
}

void CTransform::Set_Scale(const _float3& scale)
{
	_fvector newScale = XMVectorSetW(XMLoadFloat3(&scale), 0.f);
	XMStoreFloat4(&m_vPosition, newScale);
	m_bDirty = true;
}

const _float4x4& CTransform::Get_WorldMatrix()
{
	if (m_bDirty) 
		Update_Transform();
	
	return m_WorldMatrix;
}

_vector CTransform::Dir(STATE eState)
{
if (m_bDirty) 
		Update_Transform();
	_matrix worldMat = XMLoadFloat4x4(&m_WorldMatrix);
	return XMVector3Normalize(worldMat.r[static_cast<int>(eState)]);
}

void CTransform::LookAt(_fvector vAt)
{
	//목표하는 방향 벡터. 목표 - 현재
	_fvector vLookDir = XMVector3Normalize(vAt - XMLoadFloat4(&m_vPosition));

	/*
		직교 좌표계를 다시 구성하고, 거기에 내 포지션을 넣어서 회전 행렬을 만들어줄 거임.
	*/

	//내 위치 벡터
	_vector vPos = XMLoadFloat4(&m_vPosition);
	//월드업 벡터
	_fvector vWorldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	//RightVecto 구하기 - 라업룩  : 업->룩
	_fvector vRight = XMVector3Normalize(XMVector3Cross(vWorldUp, vLookDir));
	//UpVector 구하기 - 라업룩  : 룩->라
	_fvector vUp = XMVector3Cross(vLookDir, vRight);

	_matrix vRotmat = XMMatrixIdentity();
	vRotmat.r[0] = vRight;
	vRotmat.r[1] = vUp;
	vRotmat.r[2] = vLookDir;

	_vector vQuaternion = XMQuaternionRotationMatrix(vRotmat);
	XMStoreFloat4(&m_qRotation, vQuaternion);

	m_bDirty = true;
}


void CTransform::Update_Transform()
{
	_matrix matScale =	XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);

	_vector vQuaternion = XMLoadFloat4(&m_qRotation);
	_matrix matRot = XMMatrixRotationQuaternion(vQuaternion);

	_matrix matPos =	XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z);

	_matrix WorldMatrix = matScale * matRot * matPos;
	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
	m_bDirty = false;
}


CTransform* CTransform::Create()
{
	CTransform* instance = new CTransform();

	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Transform Create Failed : CTransform");
		Safe_Release(instance);
	}

	return instance;
}

CComponent* CTransform::Clone()
{
	CTransform* instance = new CTransform(*this);
	return instance;
}

void CTransform::Free()
{
	__super::Free();
}

