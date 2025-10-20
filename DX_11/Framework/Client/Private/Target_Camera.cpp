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
	
	return S_OK;
}

void CTarget_Camera::Priority_Update(_float dt)
{
	_float4 target_Pos = m_pTarget->Get_Position();
	m_pTransform->Set_Pos({target_Pos.x,target_Pos.y + 40,target_Pos.z + 30});
	m_pTransform->LookAt({target_Pos.x,target_Pos.y ,target_Pos.z});
}

void CTarget_Camera::Update(_float dt)
{
	_float4 targetPos = m_pTarget->Get_Position();
}

void CTarget_Camera::Late_Update(_float dt)
{
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