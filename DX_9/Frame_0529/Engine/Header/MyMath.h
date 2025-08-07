#pragma once

BEGIN(Engine)
class ENGINE_DLL MyMath
{
public:
	//// 轰 青纺 积己
	//D3DXMatrixLookAtLH(&m_matView, &m_vEye, &vAt, &m_vUp);
	////捧康 青纺 积己
	//D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFOV), m_fAspect, m_fNear, m_fFar);

	static void MyViewTrans(_matrix* pOut, _vec3* vEye, _vec3* vAt, _vec3* vUp);
	static void MyProjectionTrans(_matrix* pOut, float angle,  float aspect, float zNear, float zFar);
};

END