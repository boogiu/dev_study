#include "Engine_Define.h"
#include "CCamera.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "MyMath.h"

CCamera::CCamera()
	:m_pTransform(nullptr), m_fYaw(0),m_fPitch(0)
{
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

	m_fFOV = 40.f;
	m_fAspect = (float)WINCX / WINCY;
	m_fNear = 1.f;
	m_fFar = 550.f;

	return S_OK;
}

void CCamera::Update_Component(float& dt)
{
	if (!m_pTransform) {
		m_pTransform = m_pOwner->Get_Component<CTransform>();
		m_pTransform->AddRef();
	}

	// 소유자 위치를 Eye로 설정
	m_vEye = m_pTransform->Get_Pos();

	UpdateDirFromAngles();

	// At은 Eye + Look 방향
	_vec3 vAt = m_vEye + m_vLookDir;

	// 뷰 행렬 생성
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &vAt, &m_vUp);
	//투영 행렬 생성
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFOV), m_fAspect, m_fNear, m_fFar);

	//MyMath::MyViewTrans(&m_matView, &m_vEye, &vAt, &m_vUp);
	//MyMath::MyProjectionTrans(&m_matProj, D3DXToRadian(m_fFOV), m_fAspect, m_fNear, m_fFar);

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
	m_fYaw -= angle;
	_matrix rot;
	D3DXMatrixRotationAxis(&rot, &m_vUp, D3DXToRadian(m_fYaw));
	D3DXVec3TransformNormal(&m_vLookDir, &m_vLookDir, &rot);
}

void CCamera::Add_Pitch(float angle)
{
	m_fPitch += angle;
	m_fPitch = max(max(m_fPitch, 89) ,- 89);

	_vec3 vRight;
	D3DXVec3Cross(&vRight, &m_vUp, &m_vLookDir);
	D3DXVec3Normalize(&vRight, &vRight);

	_matrix rot;
	D3DXMatrixRotationAxis(&rot, &vRight, D3DXToRadian(angle));
	D3DXVec3TransformNormal(&m_vLookDir, &m_vLookDir, &rot);
	D3DXVec3TransformNormal(&m_vUp, &m_vUp, &rot); // Up 벡터도 회전
}

void CCamera::UpdateDirFromAngles() //앵글에 따른 업 벡터 다시 계산
{
	_vec3 dir; //구면 좌표계 계산 방식
	// 세타와 파이 공식(기억 안나면 ppt확인)
	dir.x = cosf(D3DXToRadian(m_fPitch)) * cosf(D3DXToRadian(m_fYaw));
	dir.y = sinf(D3DXToRadian(m_fPitch));
	dir.z = cosf(D3DXToRadian(m_fPitch)) * sinf(D3DXToRadian(m_fYaw));

	D3DXVec3Normalize(&m_vLookDir, &dir); //바라보는 방향 구함

	// Right = Up × Look
	_vec3 right;
	_vec3 worldUp = { 0.f, 1.f, 0.f };
	D3DXVec3Cross(&right, &worldUp, &m_vLookDir);//월드업과 바라보는 방향- > 우측 벡터
	D3DXVec3Normalize(&right, &right);

	// Up = Look × Right
	D3DXVec3Cross(&m_vUp, &m_vLookDir, &right);;// 우측 벡터와 바라보는 방향-> 업벡터
	D3DXVec3Normalize(&m_vUp, &m_vUp);
}

void CCamera::Add_Roll(float angle)
{
}

void CCamera::Free()
{
	Safe_Release(m_pTransform);
}