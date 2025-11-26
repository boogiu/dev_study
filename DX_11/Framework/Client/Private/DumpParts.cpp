#include "Client_Defines.h"
#include "DumpParts.h"

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

CDumpParts::CDumpParts()
{
}

CDumpParts::CDumpParts(const CDumpParts& rhs)
	:CBaseEffect(rhs)
{
}

HRESULT CDumpParts::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CDumpParts::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	// INSTANCE_INIT_DESC
	Get_Component<CStaticModel>()->Link_Model("GamePlay_Level", "distsphere.model");
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "distsphere.mat");

	Get_Component<CStaticModel>()->ShadowCast(true);
	m_vBaseVelocity = { 0,1,0,1 };

	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	pRcsMgr->Add_ResourcePath("Effect_pltcolorgroundsoillump.png", "../../Resources/Effect/pltcolorgroundsoillump.png");

	for (auto& instance : Get_Component<CMaterial>()->Get_Material_Instance()) {
		instance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
		instance->Set_Param("GradationTexture", { pRcsMgr->Load_Texture("GamePlay_Level", "Effect_pltcolorgroundsoillump.png")->Get_SRV(),"Texture2D", 0 });
		instance->Set_Param("vGradation", { &m_PaletteIndex,"float2", sizeof(_float2) });
		instance->Override_Pass("Gradation");
	}

	return S_OK;
}

void CDumpParts::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CDumpParts::Update(_float dt)
{
	m_fLifeTime += dt;
	m_vVelocity.y -= m_fGravity*dt;
	_vector velocity = XMLoadFloat4(&m_vVelocity);
	m_pTransform->Translate(velocity * dt*10);
	
	Get_Component<CObjectContainer>()->UpdateChild(dt);

	_float4 pos = Get_Position();

	if(m_vVelocity.y > 0.2f)
		m_pTransform->Rotation({ dt*3,dt * 3,dt * 3 });

	if (pos.y < 0.f)    
	{
		pos.y = 0.f;   
		m_vBaseVelocity.y *= 0.6f;
		m_vVelocity.y = m_vBaseVelocity.y;
		m_vVelocity.x *= 0.6f;
		m_vVelocity.z *= 0.6f;
	}

}

void CDumpParts::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CDumpParts::Render_GUI()
{
}

_bool CDumpParts::isEffectActive()
{
	return m_fLifeTime < 2.f;
}

void CDumpParts::Reset()
{
	m_fLifeTime = 0.f;
	m_pTransform->Set_Pos({ 0,0,0 });
	_float scale = Helper::Get_Random_Float(.3f, 1.3f);
	m_pTransform->Scale({ scale,scale,scale });

	_float velocityX = Helper::Get_Random_Float(-1.f, 1.f);
	_float velocityY = Helper::Get_Random_Float(2.f, 5.f);
	_float velocityZ = Helper::Get_Random_Float(-1.f, 1.f);

	m_vBaseVelocity = { velocityX ,velocityY ,velocityZ ,1.f };
	m_vVelocity = m_vBaseVelocity;
}

void CDumpParts::Set_DeActive()
{
	m_isAlive = false;
}

void CDumpParts::Set_ReActive(const EffectData& data)
{
	m_isAlive = true;
	Reset();
}

CDumpParts* CDumpParts::Create()
{
	CDumpParts* instance = new CDumpParts();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CDumpParts");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CDumpParts::Clone(INIT_DESC* pArg)
{
	CDumpParts* instance = new CDumpParts(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CDumpParts");
		Safe_Release(instance);
	}

	return instance;
}

void CDumpParts::Free()
{
	__super::Free();
}
