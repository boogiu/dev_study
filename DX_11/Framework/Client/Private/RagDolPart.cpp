#include "Client_Defines.h"
#include "RagDolPart.h"

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

CRagDolPart::CRagDolPart()
{
}

CRagDolPart::CRagDolPart(const CRagDolPart& rhs)
	:CBaseEffect(rhs)
{
}

HRESULT CRagDolPart::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CRagDolPart::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Get_Component<CStaticModel>()->ShadowCast(true);
	m_vBaseVelocity = { 0,1,0,1 };

	return S_OK;
}

void CRagDolPart::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CRagDolPart::Update(_float dt)
{
	m_fLifeTime += dt;
	m_vVelocity.y -= m_fGravity*dt;
	_vector velocity = XMLoadFloat4(&m_vVelocity);
	m_pTransform->Translate(velocity * dt*10);
	
	Get_Component<CObjectContainer>()->UpdateChild(dt);

	_float4 pos = Get_Position();
	if(m_vVelocity.y > 0.2f)
		m_pTransform->Rotation({ dt,dt,dt });

	if (pos.y < 0.f)    
	{
		pos.y = 0.f;   
		m_vBaseVelocity.y *= 0.6f;
		m_vVelocity.y = m_vBaseVelocity.y;
		m_vVelocity.x *= 0.6f;
		m_vVelocity.z *= 0.6f;
	}

}

void CRagDolPart::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CRagDolPart::Render_GUI()
{
}

_bool CRagDolPart::isEffectActive()
{
	return m_fLifeTime < 2.f;
}

void CRagDolPart::Reset()
{
	m_fLifeTime = 0.f;
	m_pTransform->Set_Pos({ 0,0,0 });
	_float scale = Helper::Get_Random_Float(.6f, 1.5f);
	m_pTransform->Scale({ scale,scale,scale });

	_float velocityX = Helper::Get_Random_Float(-1.f, 1.f);
	_float velocityY = Helper::Get_Random_Float(2.f, 5.f);
	_float velocityZ = Helper::Get_Random_Float(-1.f, 1.f);

	m_vBaseVelocity = { velocityX ,velocityY ,velocityZ ,1.f };
	m_vVelocity = m_vBaseVelocity;
}

void CRagDolPart::Set_DeActive()
{
	m_isAlive = false;
}

void CRagDolPart::Set_ReActive(const EffectData& data)
{
	m_isAlive = true;
	Reset();

	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	Get_Component<CStaticModel>()->Link_Model("GamePlay_Level", "rubblestone.model");
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "rubblestone.mat");

	for (auto& instance : Get_Component<CMaterial>()->Get_Material_Instance()) {
		instance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
		instance->Set_Param("DiffuseTexture", { pRcsMgr->Load_Texture("GamePlay_Level", "stoneD_mStoneD_Alb.0.dds")->Get_SRV(),"Texture2D", 0 });
		instance->Set_Param("NormalTexture", { pRcsMgr->Load_Texture("GamePlay_Level", "stoneD_mStoneD_Nrm.0.dds")->Get_SRV(),"Texture2D", 0 });
		instance->Override_Pass("Rubble");
	}
}

CRagDolPart* CRagDolPart::Create()
{
	CRagDolPart* instance = new CRagDolPart();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CRagDolPart");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CRagDolPart::Clone(INIT_DESC* pArg)
{
	CRagDolPart* instance = new CRagDolPart(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CRagDolPart");
		Safe_Release(instance);
	}

	return instance;
}

void CRagDolPart::Free()
{
	__super::Free();
}
