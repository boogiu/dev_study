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

	m_vEye = { 0.f,0.f,-10.f };
	m_vAt = { 0.f,0.f,0.f };
	m_vUp = { 0.f,1.f,0.f };
	m_fFOV = 60.f;
	m_fAspect = WINCX/WINCY;
	m_fNear = 0.1f;
	m_fFar = 100.f;
	return S_OK;
}

void CCamera::Update_Component(float dt)
{
	m_vEye = m_pOwner->Get_Component<CTransform>()->Get_Pos();
	_vec3 At = m_pOwner->Get_Component<CTransform>()->Get_Rotate();
	D3DXVec3Normalize(&m_vAt,&At);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
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