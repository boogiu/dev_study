#include "Client_Defines.h"
#include "DustEffect.h"
#include "RectModel.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "IRenderService.h"


CDustEffect::CDustEffect()
{
}

CDustEffect::CDustEffect(const CDustEffect& rhs)
	: CBaseEffect(rhs)
{
}

HRESULT CDustEffect::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CRectModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CDustEffect::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();
	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	IRenderService* pRenderSys = CGameInstance::GetInstance()->Get_RenderSystem();

	pRcsMgr->Add_ResourcePath("Effect_smoke05.png", "../../Resources/Effect/smoke05.png");
	pRcsMgr->Add_ResourcePath("Effect_smoke08.png", "../../Resources/Effect/smoke08.png");

	CMaterial* pMaterial = Get_Component<CMaterial>();
	CMaterialInstance* customInstance = CMaterialInstance::Create_Handle("Rect_Effect_Base", "Opaque", pDevice);

	pMaterial->Insert_MaterialInstance(customInstance, nullptr);
	auto MaterialDat = customInstance->Get_MaterialData();

	if (MaterialDat)
		MaterialDat->Link_Shader(G_GlobalLevelKey, "VTX_NorTexEffect.hlsl");

	customInstance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_smoke05.png", TEXTURE_TYPE::ALBEDO);
	customInstance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_smoke08.png", TEXTURE_TYPE::MIX);
	customInstance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
	customInstance->Set_Param("fRadian", { &m_fRadian,"float",sizeof(float) });
	customInstance->Set_Param("DepthTexture", { pRenderSys->Get_EngineTargetSRV("Target_Depth"),"Texture2D", 0 });
	customInstance->Override_Pass("Dust");
	customInstance->Set_Blended(true);
	m_pMaterial_Instance = customInstance;

	return S_OK;
}

void CDustEffect::Priority_Update(_float dt)
{
}

void CDustEffect::Update(_float dt)
{
	m_fRawTime += dt;                 
	float t = m_fRawTime;            
	t = clamp(t, 0.f, 1.f);

	float curved = t * t;

	m_fLifeTime = curved;

	if (m_fRawTime < 0.5f) {
		m_pTransform->AddScale({ dt*8,dt * 8 ,dt * 8 });
		m_pTransform->Translate({ dt * 2,dt*2 ,0 });
	}
	else
		m_pTransform->AddScale({ -dt,-dt ,-dt });

	m_fRadian += XMConvertToRadians(dt*15);
}

void CDustEffect::Late_Update(_float dt)
{
}

void CDustEffect::Render_GUI()
{
	if (ImGui::Button("Reset")) {
		m_fLifeTime = 0.f;
	}

	ImGui::DragFloat("time", &m_fLifeTime, 0.05);
	__super::Render_GUI();
}

_bool CDustEffect::isEffectActive()
{
	return m_fRawTime < 1.f;
}

void CDustEffect::Reset()
{
	m_fRawTime = 0.f;
	m_pTransform->Scale({ 1.f,1.f,1.f });
}

void CDustEffect::Set_DeActive()
{
	m_isAlive = false;
}

void CDustEffect::Set_ReActive(const EffectData& data)
{
	m_isAlive = true;
	Reset();
}

CDustEffect* CDustEffect::Create()
{
	CDustEffect* instance = new CDustEffect();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CDustEffect");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CDustEffect::Clone(INIT_DESC* pArg)
{
	CDustEffect* instance = new CDustEffect(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CDustEffect");
		Safe_Release(instance);
	}

	return instance;
}

void CDustEffect::Free()
{
	__super::Free();
}
