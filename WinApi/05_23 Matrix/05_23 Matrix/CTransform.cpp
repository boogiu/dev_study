#include "pch.h"
#include "CTransform.h"

CTransform::CTransform()
	:m_pParent(nullptr), m_vPivot{}, m_vOrbit{}
{
	m_tRect.m_vCenterPosLocal = { 0,0,0 };
	m_tRect.tLT.m_vLocalPos = { -0.5f, -0.5f, 0.f };
	m_tRect.tLB.m_vLocalPos = { -0.5f,  0.5f, 0.f };
	m_tRect.tRT.m_vLocalPos = { 0.5f, -0.5f, 0.f };
	m_tRect.tRB.m_vLocalPos = { 0.5f,  0.5f, 0.f };
	D3DXMatrixIdentity(&m_WorldMat); //초기화

}

CTransform::~CTransform()
{
}

void CTransform::Initialize()
{
}

void CTransform::Update(float dt)
{
	D3DXMATRIX matWorld, matScale, matRotX, matRotY, matRotZ, matTrans; //월드 포지션 / 크기/자전/이동

	D3DXMatrixIdentity(&matWorld); //초기화
	//크기
	D3DXMatrixScaling(&matScale, m_tRect.m_vScale.x, m_tRect.m_vScale.y, m_tRect.m_vScale.z);

	//자전
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_tRect.m_vRotate.x));
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_tRect.m_vRotate.y));
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_tRect.m_vRotate.z));

	D3DXMatrixTranslation(&matTrans,
		m_tRect.m_vCenterPosLocal.x,
		m_tRect.m_vCenterPosLocal.y,
		m_tRect.m_vCenterPosLocal.z);

	// 4. 공전 처리
	D3DXMATRIX matPivotR, matPivotT, matPivotInvT;
	D3DXMatrixRotationZ(&matPivotR, D3DXToRadian(m_vOrbit.z)); // 공전 회전
	D3DXMatrixTranslation(&matPivotT, m_vPivot.x, m_vPivot.y, m_vPivot.z); // 중심점
	D3DXMatrixTranslation(&matPivotInvT, -m_vPivot.x, -m_vPivot.y, -m_vPivot.z);

	D3DXMATRIX matOrbital = matPivotInvT * matPivotR * matPivotT;

	matWorld = matScale * matRotX * matRotY * matRotZ * matTrans * matOrbital;

	if (m_pParent) {
		D3DXMATRIX parentMatTrans;
		D3DXMatrixTranslation(&parentMatTrans,
			m_pParent->Get_Position_Local().x,
			m_pParent->Get_Position_Local().y,
			m_pParent->Get_Position_Local().z);
			matWorld *= parentMatTrans;
	}
	// 예시: 위치 결과를 다시 적용
	D3DXVec3TransformCoord(&m_tRect.m_vCenterPosWorld, &m_tRect.m_vCenterPosLocal, &matWorld);
	D3DXVec3TransformCoord(&m_tRect.tLT.m_vWorldPos, &m_tRect.tLT.m_vLocalPos, &matWorld);
	D3DXVec3TransformCoord(&m_tRect.tLB.m_vWorldPos, &m_tRect.tLB.m_vLocalPos, &matWorld);
	D3DXVec3TransformCoord(&m_tRect.tRT.m_vWorldPos, &m_tRect.tRT.m_vLocalPos, &matWorld);
	D3DXVec3TransformCoord(&m_tRect.tRB.m_vWorldPos, &m_tRect.tRB.m_vLocalPos, &matWorld);
	m_WorldMat = matWorld;
}

void CTransform::Set_Scale(float _x, float _y, float _z)
{
	m_tRect.m_vScale = { _x,_y,_z };
}

void CTransform::Set_Position(float _x, float _y, float _z)
{
	m_tRect.m_vCenterPosLocal = { _x,_y,_z };
}

void CTransform::Set_Position(D3DXVECTOR3 _pos)
{
	m_tRect.m_vCenterPosLocal = _pos;
}

void CTransform::Set_Rotation(float _x, float _y, float _z)
{
	m_tRect.m_vRotate = { _x,_y,_z };
}

void CTransform::Add_Position(float _x, float _y, float _z)
{
	m_tRect.m_vCenterPosLocal.x += _x;
	m_tRect.m_vCenterPosLocal.y += _y;
	m_tRect.m_vCenterPosLocal.z += _z;
}

void CTransform::Add_Position(D3DXVECTOR3 vDir)
{
	m_tRect.m_vCenterPosLocal += vDir;
}

void CTransform::Add_Rotate(D3DXVECTOR3 vRot)
{
	m_tRect.m_vRotate += vRot;
}

void CTransform::Set_Pivot(float _x, float _y, float _z)
{
	m_vPivot.x = _x;
	m_vPivot.y = _y;
	m_vPivot.z = _z;
}

void CTransform::Set_Orbit(float _x, float _y, float _z)
{
	m_vOrbit.x = _x;
	m_vOrbit.y = _y;
	m_vOrbit.z = _z;
}

void CTransform::Set_Pivot(D3DXVECTOR3 pivot)
{
	m_vPivot = pivot;
}

void CTransform::Set_Orbit(D3DXVECTOR3 orbit)
{
	m_vOrbit = orbit;
}

void CTransform::Add_RotateX(float x)
{
	m_tRect.m_vRotate.x += x;
}

void CTransform::Add_RotateY(float y)
{
	m_tRect.m_vRotate.y += y;
}

void CTransform::Add_RotateZ(float z)
{
	m_tRect.m_vRotate.z += z;
}

D3DXVECTOR3 CTransform::Get_Scale()
{
	return m_tRect.m_vScale;
}

D3DXVECTOR3 CTransform::Get_Position_Local()
{
	return m_tRect.m_vCenterPosLocal;
}

D3DXVECTOR3 CTransform::Get_Position_World()
{
	return m_tRect.m_vCenterPosWorld;
}

D3DXVECTOR3 CTransform::Get_Rotation()
{
	return m_tRect.m_vRotate;
}

RECTANGLE CTransform::Get_Rectangle()
{
	return m_tRect;
}

void CTransform::Set_Parent(CTransform* parent)
{
	m_pParent = parent; 
}

CTransform* CTransform::Get_Parent()
{
	return m_pParent;
}

D3DXMATRIX CTransform::Get_WorldMatrix()
{
	return m_WorldMat;
}

void CTransform::Late_Update(float dt)
{
}
