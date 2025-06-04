#include "Engine_Define.h"
#include "CCamera.h"
#include "CGameObject.h"
#include "CTransform.h"

CCamera::CCamera()
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
	m_vAt = { 0.f,0.f,0.f };
	m_vUp = { 0.f,1.f,0.f };
	m_fFOV = 60.f;
	m_fAspect = (float)WINCX / WINCY;
	m_fNear = 1.f;
	m_fFar = 500.f;
	return S_OK;
}

void CCamera::Update_Component(float dt)
{
	// Transform 기준 위치 계산 (공전 삭제)
	_vec3 tmp(0.f, 0.f, 0.f);

	D3DXVec3TransformCoord(
		&m_vEye,
		&tmp,
		&m_pOwner->Get_Component<CTransform>()->Get_WorldMatrix());
	//주인의 몸체와 동일
	
	// 카메라가 보는 지점: 피벗 (일반적으로 타겟의 위치)
	m_vAt = m_pOwner->Get_Component<CTransform>()->Get_Look();

	// Look 방향 벡터
	_vec3 Look = m_vAt - m_vEye;
	D3DXVec3Normalize(&Look, &Look);

	// 오른쪽 벡터: Up(이전 프레임 기준) × Look
	_vec3 right;

	D3DXVec3Cross(&right, &m_vUp, &Look);
	D3DXVec3Normalize(&right, &right);

	// 위쪽 벡터: Look × Right
	D3DXVec3Cross(&m_vUp, &Look, &right);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	// 뷰 행렬 생성
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);

	// 투영 행렬 생성
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFOV), m_fAspect, m_fNear, m_fFar);
}
void CCamera::LateUpdate_Component(float dt)
{
}


CComponent* CCamera::Clone() const
{
	return nullptr;
}

void CCamera::Set_View(_vec3 _vEye, _vec3 _vAt, _vec3 _vUP)
{
	m_vEye = _vEye;
	m_vAt = _vAt;
	m_vUp = _vUP;
}

void CCamera::Set_Proj(float _FOV, float _aspect, float _near, float _far)
{
	m_fFOV = _FOV;
	m_fAspect = _aspect;
	m_fNear = _near;
	m_fFar = _far;
}

void CCamera::Free()
{
	
}