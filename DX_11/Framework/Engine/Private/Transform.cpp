#include "Transform.h"

CTransform::CTransform()
{
}

CTransform::CTransform(const CTransform& rhs)
	:CComponent(rhs)
{
}

HRESULT CTransform::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTransform::Initialize(INIT_DESC* pArg)
{
	if (pArg == nullptr)
		return S_OK;

	TRANSFORM_DESC* desc = static_cast<TRANSFORM_DESC*>(pArg);
	m_fSpeedPerSec = desc->SpeedPerSec;
	m_fRotatePerSec = desc->RotatePerSec;

	return S_OK;
}


CTransform* CTransform::Create()
{
	CTransform* instance = new CTransform();

	if (FAILED(instance->Initialize_Prototype())) {
		Safe_Release(instance);
	}

	return instance;
}

CComponent* CTransform::Clone()
{
	CTransform* instance = new CTransform(*this);
	return instance;
}

void CTransform::Free()
{
	__super::Free();
}

