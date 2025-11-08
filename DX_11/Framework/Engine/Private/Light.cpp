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
	if(pArg != nullptr){
	LIGHT_INIT_DESC* desc = static_cast<LIGHT_INIT_DESC*>(pArg);
	m_eType = desc->eType;
	m_Light.fLightRange = desc->fRange;
	m_Light.vLightAmbient = desc->vAmbient;
	m_Light.vLightDiffuse = desc->vDiffuse;
	m_Light.vLightDirection = desc->vDirection;
	m_Light.vLightSpecular = desc->vSpecular;
	}
	m_ID= CGameInstance::GetInstance()->Get_LightMgr()->Register_Light(this);
	return S_OK;
}

void CLight::Render_GUI()
{
}

void CLight::Set_Desc(const LIGHT_DESC& desc, LIGHT_TYPE eType)
{
	m_Light = desc;
	m_eType = eType;
}

_float4 CLight::Get_Position()
{
	Get_Desc();
	return m_Light.vLightPosition;
}

LIGHT_DESC* CLight::Get_Desc()
{
	m_Light.vLightPosition = m_pOwner->Get_Position();
	return &m_Light;
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
		instance->m_Light =desc;
	}
	return instance;
}

CLight* CLight::Create()
{
	CLight* instance = new CLight();
	if (FAILED(instance->Initialize_Prototype()))
	{
		Safe_Release(instance);
		MSG_BOX("Light Comp Failed To Create : CLight");
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
	//CGameInstance::GetInstance()->Get_LightMgr()->UnRegister_Light(m_ID);
}
