#include "Engine_Define.h"
#include "CLight.h"
#include "CTransform.h"
#include "CGameObject.h"

CLight::CLight()
{
	ZeroMemory(&m_tLight, sizeof(D3DLIGHT9));
	m_eUpdate = COM_UPDATE::DYNAMIC;

}

CLight::~CLight()
{
}

CLight* CLight::Create(LIGHT_TYPE type)
{
	CLight* instance = new CLight;

	if (FAILED(instance->Ready_Component())) {
		Safe_Release(instance);
		instance = nullptr;
	}
	instance->Set_Type(type);
	return instance;
}


HRESULT CLight::Ready_Component()
{
	return S_OK;
}

void CLight::Update_Component(float& dt)
{
	CTransform* ownerTrans = m_pOwner->Get_Component<CTransform>();
	
	if (!ownerTrans) return;

	_vec3 worldPos;
	_vec3 tmp = {0.f,0.f,0.f};

	D3DXVec3TransformCoord(&worldPos, &tmp, &ownerTrans->Get_WorldMatrix());

	m_tLight.Position = worldPos; 
}

void CLight::LateUpdate_Component(float& dt)
{
}

CComponent* CLight::Clone() const
{
	return nullptr;
}

void CLight::Set_Type(LIGHT_TYPE light)
{
	switch (light)
	{
	case Engine::LIGHT_TYPE::DIRECTION:
		m_tLight.Type = D3DLIGHT_DIRECTIONAL;
		break;
	case Engine::LIGHT_TYPE::POINT:
		m_tLight.Type = D3DLIGHT_POINT;
		break;
	case Engine::LIGHT_TYPE::SPOTLIGHT:
		m_tLight.Type = D3DLIGHT_SPOT;
		break;
	default:
		break;
	}
}

void CLight::Set_LightDesc(D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, float range, float att0, float att1, float att2)
{
		m_tLight.Ambient = ambient;
		m_tLight.Diffuse = diffuse;
		m_tLight.Specular = specular;

		m_tLight.Range = range;
		m_tLight.Attenuation0 = att0;
		m_tLight.Attenuation1 = att1;
		m_tLight.Attenuation2 = att2;
}

void CLight::Set_Angle(float theta, float phi)
{
	m_tLight.Theta = D3DXToRadian(theta);
	m_tLight.Phi = D3DXToRadian(phi);
}

void CLight::Set_Dir(_vec3 dir)
{
	m_tLight.Direction = dir;
}


void CLight::Free()
{
}
