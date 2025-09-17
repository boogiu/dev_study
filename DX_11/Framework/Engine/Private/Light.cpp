#include "Light.h"
#include "GameInstance.h"
#include	"ILightService.h"
#include "GameObject.h"
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

	LIGHT_INIT_DESC* desc = static_cast<LIGHT_INIT_DESC*>(pArg);
	m_Light.eType = desc->eType;
	m_Light.fRange = desc->fRange;
	m_Light.vAmbient = desc->vAmbient;
	m_Light.vDiffuse = desc->vDiffuse;
	m_Light.vDirection = desc->vDirection;
	m_Light.vSpecular = desc->vSpecular;

	m_ID= CGameInstance::GetInstance()->Get_LightMgr()->Register_Light(this);
	return S_OK;
}

void CLight::Render_GUI()
{
}

void CLight::Set_Desc(const LIGHT_DESC& desc)
{
	m_Light = desc;
}

_float4 CLight::Get_Position()
{
	return m_pOwner->Get_Position();
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
	CGameInstance::GetInstance()->Get_LightMgr()->UnRegister_Light(m_ID);
}
