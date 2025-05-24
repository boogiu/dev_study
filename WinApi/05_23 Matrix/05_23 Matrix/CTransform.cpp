#include "pch.h"
#include "CTransform.h"

CTransform::CTransform()
{
}

CTransform::~CTransform()
{
}

void CTransform::Initialize()
{
}

void CTransform::Update(float dt)
{

}

void CTransform::Set_Scale(float _x, float _y, float _z)
{
	m_vScale.x = _x;
	m_vScale.x = _y;
	m_vScale.x = _z;
}

void CTransform::Set_Position(float _x, float _y, float _z)
{
	m_vPos.x = _x;
	m_vPos.x = _y;
	m_vPos.x = _z;
}

void CTransform::Set_Rotation(float _x, float _y, float _z)
{
	m_vRotate.x = _x;
	m_vRotate.x = _y;
	m_vRotate.x = _z;
}

D3DXVECTOR3 CTransform::Get_Scale()
{
	return D3DXVECTOR3();
}

D3DXVECTOR3 CTransform::Get_Position()
{
	return D3DXVECTOR3();
}

D3DXVECTOR3 CTransform::Get_Rotation()
{
	return D3DXVECTOR3();
}
