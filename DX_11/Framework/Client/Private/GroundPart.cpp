#include "Client_Defines.h"
#include "GroundPart.h"

#include "RectModel.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "ObjectContainer.h"
#include "StaticModel.h"

#include "IRenderService.h"
#include "Texture.h"

#include "Helper_Func.h"

CGroundPart::CGroundPart()
{
}

CGroundPart::CGroundPart(const CGroundPart& rhs)
	:CBaseEffect(rhs)
{
}

HRESULT CGroundPart::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CGroundPart::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Get_Component<CStaticModel>()->ShadowCast(true);


	m_vBasePos = Get_Position();
	float rotY = atan2f(-m_vBasePos.x,-m_vBasePos.z);
	m_pTransform->Rotation({ 0, rotY, 0 });

	return S_OK;
}

void CGroundPart::Priority_Update(_float dt)
{
}

void CGroundPart::Update(_float dt)
{
	if (m_fLifeTime > 1.f)
		return;

	_float speed = dt * 2.5f;
	m_pTransform->AddScale({ speed,speed,speed });
	m_fLifeTime += dt;
}

void CGroundPart::Late_Update(_float dt)
{
}

void CGroundPart::Render_GUI()
{
}

_bool CGroundPart::isEffectActive()
{
	return m_fLifeTime < 2.f;
}

void CGroundPart::Reset()
{
	m_pTransform->Scale({ 1.f,1.f,1.f });
	m_fLifeTime = 0.f;
}

void CGroundPart::Set_DeActive()
{
	m_isAlive = false;
}

void CGroundPart::Set_ReActive(const EffectData& data)
{
	m_isAlive = true;
	Reset();
}

CGroundPart* CGroundPart::Create()
{
	CGroundPart* instance = new CGroundPart();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CGroundPart");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CGroundPart::Clone(INIT_DESC* pArg)
{
	CGroundPart* instance = new CGroundPart(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CGroundPart");
		Safe_Release(instance);
	}

	return instance;
}

void CGroundPart::Free()
{
	__super::Free();
}
