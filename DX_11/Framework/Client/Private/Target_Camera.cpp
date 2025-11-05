#include "Client_Defines.h"
#include "Target_Camera.h"
#include "Camera.h"

CTarget_Camera::CTarget_Camera()
{
}

CTarget_Camera::CTarget_Camera(const CTarget_Camera& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTarget_Camera::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CCamera>();
	return S_OK;
}

HRESULT CTarget_Camera::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	TARGET_CAM_DESC* pDesc = static_cast<TARGET_CAM_DESC*>(pArg);
	m_pTarget = pDesc->pTarget;
	_float4 TagetPos = m_pTarget->Get_Position();
	
	m_pTransform->Set_Pos({ TagetPos.x, 50,	TagetPos.z+ 50 });
	m_pTransform->LookAt({ TagetPos.x, 0,	TagetPos.z + 10 });
	m_vOffset = { 0,50,50,0 };

	m_vZoomInOffset = { 0,15,50,0 };
	m_fCurrentLookY = 0;
	//m_vOffset = m_pTarget->Get_Position();


	return S_OK;
}

void CTarget_Camera::Priority_Update(_float dt)
{
	switch (m_eState)
	{
	case Client::CTarget_Camera::FOLLOW:
		Follow_Target(dt);
		break;
	case Client::CTarget_Camera::ZOOM_IN:
		Zoom_In(dt);
		break;
	case Client::CTarget_Camera::ZOOM_OUT:
		Zoom_Out(dt);
		break;
	default:
		break;
	}

}

void CTarget_Camera::Update(_float dt)
{
	_float4 targetPos = m_pTarget->Get_Position();
}

void CTarget_Camera::Late_Update(_float dt)
{
}

void CTarget_Camera::Execute_ZoomIn()
{
	m_eState = ZOOM_IN;
}

void CTarget_Camera::Release_ZoomIn()
{
	m_eState = ZOOM_OUT;
}

void CTarget_Camera::Zoom_In(_float dt)
{
	_vector target_Pos = m_pTarget->Get_Component<CTransform>()->Get_Pos();
	_vector Offset = XMLoadFloat4(&m_vZoomInOffset);
	_vector cam_Pos = m_pTransform->Get_Pos(); //Now Pso

	//Move Lerp
	_vector MoveDir = XMVectorLerp(cam_Pos, target_Pos + Offset, dt * 5);

	_float3 DstPos;
	XMStoreFloat3(&DstPos, MoveDir);

	_float3 LookPos;
	XMStoreFloat3(&LookPos, target_Pos);
	//보는 방향은 그대로 유지하되, y축만 올려야함. (룩앳포즈는 올리고, 현재 포즈는 내린다)

	if (m_fCurrentLookY < 15)
	{
		m_fCurrentLookY += dt*25;
	}
	if (m_fCurrentLookY > 15)
	{
		m_fCurrentLookY = 15;
	}
	m_pTransform->LookAt({ LookPos.x, m_fCurrentLookY,	LookPos.z + 10 });

	m_pTransform->Set_Pos(DstPos);

	Get_Component<CCamera>()->Lerp_FOV(40, dt*1.5);
}

void CTarget_Camera::Zoom_Out(_float dt)
{
	_vector target_Pos = m_pTarget->Get_Component<CTransform>()->Get_Pos();
	_vector Offset = XMLoadFloat4(&m_vOffset);
	_vector cam_Pos = m_pTransform->Get_Pos(); //Now Pso

	//Move Lerp
	_vector MoveDir = XMVectorLerp(cam_Pos, target_Pos + Offset, dt * 5);

	_float3 DstPos;
	XMStoreFloat3(&DstPos, MoveDir);

	_float3 LookPos;
	XMStoreFloat3(&LookPos, target_Pos);

	if (m_fCurrentLookY > 0)
	{
		m_fCurrentLookY -= dt * 25;
		m_pTransform->LookAt({ LookPos.x, m_fCurrentLookY,	LookPos.z + 10 });
	}
	if (m_fCurrentLookY < 0)
	{
		m_fCurrentLookY = 0;
		m_eState = FOLLOW;
	}

	m_pTransform->Set_Pos(DstPos);
	Get_Component<CCamera>()->Lerp_FOV(60, dt);

}

void CTarget_Camera::Follow_Target(_float dt)
{
	_vector target_Pos = m_pTarget->Get_Component<CTransform>()->Get_Pos();
	_vector Offset = XMLoadFloat4(&m_vOffset);
	_vector cam_Pos = m_pTransform->Get_Pos(); //Now Pso

	//Move Lerp
	_vector MoveDir=XMVectorLerp(cam_Pos, target_Pos+ Offset,dt*10);

	_float3 DstPos;
	XMStoreFloat3(&DstPos, MoveDir);

	m_pTransform->Set_Pos(DstPos);
}

void CTarget_Camera::Render_GUI()
{
	__super::Render_GUI();
	ImGui::InputFloat3("Offset", reinterpret_cast<_float*>(&m_vOffset));
}

CTarget_Camera* CTarget_Camera::Create()
{
	CTarget_Camera* instance = new CTarget_Camera();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CTarget_Camera");
		Safe_Release(instance);
	}

	return instance;
}


CGameObject* CTarget_Camera::Clone(INIT_DESC* pArg)
{
	CTarget_Camera* instance = new CTarget_Camera(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CTarget_Camera");
		Safe_Release(instance);
	}

	return instance;
}

void CTarget_Camera::Free()
{
	__super::Free();
}