#include "Client_Defines.h"
#include "SunCam.h"
#include "Camera.h"
#include "Light.h"

CSunCam::CSunCam()
{
}

CSunCam::CSunCam(const CSunCam& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSunCam::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CCamera>();
	return S_OK;
}

HRESULT CSunCam::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	m_pTransform->LookAt({ 650 ,0,650 });
	return S_OK;
}

void CSunCam::Priority_Update(_float dt)
{
}

void CSunCam::Update(_float dt)
{
	if (m_pTarget) {
		_float4 pos = m_pTarget->Get_Position();
		pos.z += 70;
		pos.y += m_InitialHeight;
		m_pTransform->Set_Pos(pos);
		m_pTransform->LookAt(m_pTarget->Get_Component<CTransform>()->Get_Pos());
	}
}

void CSunCam::Late_Update(_float dt)
{
}

void CSunCam::Render_GUI()
{
	__super::Render_GUI();
}

CSunCam* CSunCam::Create()
{
	CSunCam* instance = new CSunCam();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CSunCam");
		Safe_Release(instance);
	}

	return instance;
}


CGameObject* CSunCam::Clone(INIT_DESC* pArg)
{
	CSunCam* instance = new CSunCam(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CSunCam");
		Safe_Release(instance);
	}

	return instance;
}

void CSunCam::Free()
{
	__super::Free();
}
