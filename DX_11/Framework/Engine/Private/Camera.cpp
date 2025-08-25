#include "Camera.h"
#include "GameObject.h"

CCamera::CCamera()
{
}

CCamera::CCamera(const CCamera& rhs)
	:CComponent(rhs)
{
}

HRESULT CCamera::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CCamera::Initialize(COMPONENT_DESC* pArg)
{
	if (pArg == nullptr)
		return S_OK;

	CAMERA_DESC* Camera = static_cast<CAMERA_DESC*>(pArg);
	m_pTransform = m_pOwner->Get_Component<CTransform>();
	Safe_AddRef(m_pTransform);

	return S_OK;
}

CCamera* CCamera::Create()
{
	CCamera* instance = new CCamera();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Camera Create Failed : CCamera");
		Safe_Release(instance);
	}
	return instance;
}

CComponent* CCamera::Clone()
{
	CCamera* instance = new CCamera(*this);
	return instance;
}

void CCamera::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
}
