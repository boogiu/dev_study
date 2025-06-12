#include "Engine_Define.h"
#include "CCamera.h"
#include "CGameObject.h"
#include "CTransform.h"

CCamera::CCamera()
	:m_pTransform(nullptr)
{
	m_eUpdate = COM_UPDATE::DYNAMIC;
}

CCamera::~CCamera()
{
}

CCamera* CCamera::Create()
{
	CCamera* instance = new CCamera;

	if (FAILED(instance->Ready_Component())) {
		Safe_Release(instance);
		return nullptr;
	}

	return instance;
}

HRESULT CCamera::Ready_Component()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	m_vEye = { 0.f,0.f,-20.f };
	m_vUp = { 0.f,1.f,0.f };
	m_vLookDir = { 0.f, 0.f, 1.f }; 

	m_fFOV = 60.f;
	m_fAspect = (float)WINCX / WINCY;
	m_fNear = 1.f;
	m_fFar = 500.f;
	return S_OK;
}

void CCamera::Update_Component(float& dt)
{
	if (!m_pTransform)
		m_pTransform = m_pOwner->Get_Component<CTransform>();

	// 소유자 위치를 Eye로 설정
	m_vEye = m_pTransform->Get_Pos();

	// At은 Eye + Look 방향
	_vec3 vAt = m_vEye + m_vLookDir;

	// 오른쪽 벡터 계산
	_vec3 vRight;
	D3DXVec3Cross(&vRight, &m_vUp, &m_vLookDir);
	D3DXVec3Normalize(&vRight, &vRight);

	// Up 벡터 다시 계산
	D3DXVec3Cross(&m_vUp, &m_vLookDir, &vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	// 뷰 행렬 생성
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &vAt, &m_vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFOV), m_fAspect, m_fNear, m_fFar);
}
void CCamera::LateUpdate_Component(float& dt)
{
}


CComponent* CCamera::Clone() const
{
	return nullptr;
}

void CCamera::Set_View(_vec3 _vEye, _vec3 _vAt, _vec3 _vUP)
{
	m_vEye = _vEye;
	m_vUp = _vUP;
}

void CCamera::Set_Proj(float _FOV, float _aspect, float _near, float _far)
{
	m_fFOV = _FOV;
	m_fAspect = _aspect;
	m_fNear = _near;
	m_fFar = _far;
}
void CCamera::Add_Yaw(float angle)
{
	_matrix rot;
	D3DXMatrixRotationAxis(&rot, &m_vUp, D3DXToRadian(angle));
	D3DXVec3TransformNormal(&m_vLookDir, &m_vLookDir, &rot);
}

void CCamera::Add_Pitch(float angle)
{
	_vec3 vRight;
	D3DXVec3Cross(&vRight, &m_vUp, &m_vLookDir);
	D3DXVec3Normalize(&vRight, &vRight);

	_matrix rot;
	D3DXMatrixRotationAxis(&rot, &vRight, D3DXToRadian(angle));
	D3DXVec3TransformNormal(&m_vLookDir, &m_vLookDir, &rot);
	D3DXVec3TransformNormal(&m_vUp, &m_vUp, &rot); // Up 벡터도 회전
}


void CCamera::Add_Roll(float angle)
{
}

void CCamera::Free()
{
	
}