#include "Client_Defines.h"
#include "SmokeEffect.h"

#include "RectModel.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "ObjectContainer.h"
#include "Helper_Func.h"
#include "Texture.h"
#include "InstanceModel.h"

CSmokeEffect::CSmokeEffect()
{
}

CSmokeEffect::CSmokeEffect(const CSmokeEffect& rhs)
	: CBaseEffect(rhs)
{
}

HRESULT CSmokeEffect::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
	return S_OK;
}

HRESULT CSmokeEffect::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	m_ParicleCount = 5;

	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();
	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	IRenderService* pRenderSys = CGameInstance::GetInstance()->Get_RenderSystem();

	pRcsMgr->Add_ResourcePath("Effect_smoke04.png", "../../Resources/Effect/smoke04.png");
	pRcsMgr->Add_ResourcePath("Effect_smoke04_nrm.png", "../../Resources/Effect/smoke04_nrm.png");
	pRcsMgr->Add_ResourcePath("Effect_glow01.png", "../../Resources/Effect/glow01.png");

	for (size_t i = 0; i < m_ParicleCount; i++)
	{	
		CGameObject* pObj = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_BaseEffect"}).Build("smoke");
		pObj->Add_Component<CRectModel>();
		CMaterial* pMaterial = pObj->Add_Component<CMaterial>();
		CMaterialInstance* customInstance = CMaterialInstance::Create_Handle("Rect_Effect_Base", "Dust", pDevice);
		pMaterial->Insert_MaterialInstance(customInstance, nullptr);
		auto MaterialDat = customInstance->Get_MaterialData();

		if (MaterialDat)
			MaterialDat->Link_Shader(G_GlobalLevelKey, "VTX_NorTexEffect.hlsl");

		customInstance->Get_MaterialData()->Link_Texture("GamePlay_Level", "Effect_smoke04.png", TEXTURE_TYPE::ALBEDO);
		customInstance->Get_MaterialData()->Link_Texture("GamePlay_Level", "Effect_smoke04_nrm.png", TEXTURE_TYPE::NORMAL);
		customInstance->Get_MaterialData()->Link_Texture("GamePlay_Level", "Effect_glow01.png", TEXTURE_TYPE::MIX);
		customInstance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
		customInstance->Set_Param("DepthTexture", { pRenderSys->Get_EngineTargetSRV("Target_Depth"),"Texture2D", 0 });
		customInstance->Override_Pass("Smoke");
		customInstance->Set_Blended(true);

		Get_Component<CObjectContainer>()->Add_Child(pObj,true);
		m_Clouds.push_back(dynamic_cast<CBaseEffect*>(pObj));
	}

	m_fBaseSpeed = 11.f;
	return S_OK;
}

void CSmokeEffect::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CSmokeEffect::Update(_float dt)
{
	m_fLifeTime += dt;
	float t = std::clamp(m_fLifeTime, 0.f, 1.f);

	float s;
	if (t < 0.3f)                     
		s = powf(t / 0.3f, 2.0f);
	else if (t < 0.7f)                
		s = 1.0f;
	else                              
	{
		float u = (t - 0.7f) / 0.3f;      
		s = 1.0f - powf(u, 2.0f) * 0.4f;
	}

	float phase = XM_PIDIV2;

	for (size_t i = 0; i < m_ParicleCount; i++)
	{
		float angle = XM_2PI * (float)((i * 2) % m_ParicleCount) / m_ParicleCount + phase;
		auto transform = m_Clouds[i]->Get_Component<CTransform>();
		auto Lerp = [](_float a, _float b, _float t) -> _float{return a + (b - a) * t;};
		float scale = Lerp(1.f, 14.0f, s); // 가장 작을 때 0.4, 최대 2.0
		transform->Scale({ scale, scale, scale });

		// 위치 퍼짐
		transform->Set_Pos({
			cosf(angle) * t * 4.5f,
			4.5f,
			sinf(angle) * t * 4.5f
			});
	}

	float damping = powf(0.98f, dt * 60.f);
	m_fSpeed *= damping;

	Get_Component<CObjectContainer>()->UpdateChild(dt);
}


void CSmokeEffect::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CSmokeEffect::Render_GUI()
{
	__super::Render_GUI();
}

_bool CSmokeEffect::isEffectActive()
{

	return m_fLifeTime < 1.f;
}

void CSmokeEffect::Reset()
{
	m_fLifeTime = 0.12f;


	for (size_t i = 0; i < m_ParicleCount; i++)
	{
		m_Clouds[i]->Get_Component<CTransform>()->Set_Pos({ 0,0,0});
		m_Clouds[i]->Get_Component<CTransform>()->Scale({ 1.f,1.f,1.f });
	}
	m_fSpeed = m_fBaseSpeed;
}

void CSmokeEffect::Set_DeActive()
{
	m_isAlive = false;
}

void CSmokeEffect::Set_ReActive(const EffectData& data)
{
	m_isAlive = true;
	Reset();
	m_pTransform->Scale({ 1.f,1.f,1.f });

}

CSmokeEffect* CSmokeEffect::Create()
{
	CSmokeEffect* instance = new CSmokeEffect();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CSmokeEffect");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CSmokeEffect::Clone(INIT_DESC* pArg)
{
	CSmokeEffect* instance = new CSmokeEffect(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CSmokeEffect");
		Safe_Release(instance);
	}

	return instance;
}

void CSmokeEffect::Free()
{
	__super::Free();
}
