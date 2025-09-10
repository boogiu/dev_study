#include "Light.h"
#include "GameInstance.h"
#include	"ILightService.h"

CLight::CLight()
{
}

CLight::CLight(const CLight& rhs)
	: CComponent(rhs), m_Light{rhs.m_Light}
{
}

HRESULT CLight::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLight::Initialize(COMPONENT_DESC* pArg)
{
	CGameInstance::GetInstance()->Get_LightMgr()->Register_Light(&m_Light);

	return S_OK;
}

void CLight::Render_GUI()
{
}

void CLight::Set_Desc(const LIGHT_DESC& desc)
{
	m_Light = desc;
}

CLight* CLight::Create(const LIGHT_DESC& desc)
{
	CLight* instance = new CLight();
	if (FAILED(instance->Initialize_Prototype()))
	{
		Safe_Release(instance);
		MSG_BOX("Light Comp Failed To Create : CLight");
	}
	else {
		instance->Set_Desc(desc);
	}
	return instance;
}

CComponent* CLight::Clone()
{
	return new CLight(*this);
}

void CLight::Free()
{
	__super::Free();
	CGameInstance::GetInstance()->Get_LightMgr()->UnRegister_Light(&m_Light);
}
