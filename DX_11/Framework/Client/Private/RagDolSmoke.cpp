#include "Client_Defines.h"
#include "RagDolSmoke.h"
#include "RectModel.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "Helper_Func.h"
#include "ObjectContainer.h"

#include "CraftChildEffect.h"
#include "CraftTwinkle.h"

CRagDolSmoke::CRagDolSmoke()
{
}

CRagDolSmoke::CRagDolSmoke(const CRagDolSmoke& rhs)
	: CBaseEffect(rhs)
{
}

HRESULT CRagDolSmoke::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CRectModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CRagDolSmoke::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();
	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	IRenderService* pRenderSys = CGameInstance::GetInstance()->Get_RenderSystem();

	pRcsMgr->Add_ResourcePath("Effect_smoke07.png", "../../Resources/Effect/smoke07.png");
	pRcsMgr->Add_ResourcePath("Effect_smoke08.png", "../../Resources/Effect/smoke08.png");

	CMaterial* pMaterial = Get_Component<CMaterial>();
	CMaterialInstance* customInstance = CMaterialInstance::Create_Handle("Rect_Effect_Base", "Craft", pDevice);

	pMaterial->Insert_MaterialInstance(customInstance, nullptr);
	auto MaterialDat = customInstance->Get_MaterialData();

	if (MaterialDat)
		MaterialDat->Link_Shader(G_GlobalLevelKey, "VTX_NorTexEffect.hlsl");

	customInstance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_smoke07.png", TEXTURE_TYPE::ALBEDO);
	customInstance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_smoke08.png", TEXTURE_TYPE::MIX);

	customInstance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
	customInstance->Set_Param("fRadian", { &m_fRadian,"float",sizeof(float) });
	customInstance->Set_Param("fAtlasIndex", { &m_AtlasIndex,"float2",sizeof(_float2) });
	customInstance->Set_Param("fAtlasScale", { &m_AtlasScale,"float2",sizeof(_float2) });
	customInstance->Set_Param("DepthTexture", { pRenderSys->Get_EngineTargetSRV("Target_Depth"),"Texture2D", 0 });

	customInstance->Override_Pass("FrameDust");
	customInstance->Set_Blended(true);

	m_pMaterial_Instance = customInstance;
	m_AtlasScale = { 1 / 2.f,1 / 2.f };

	return S_OK;
}

void CRagDolSmoke::Priority_Update(_float dt)
{

}

void CRagDolSmoke::Update(_float dt)
{
	m_fLifeTime += dt;
	float size = cosf(m_fLifeTime * 5) * 0.5f;
	m_pTransform->AddScale({ size,size  ,0.f });

	m_fAtlasFrame += dt * 16;
	int frame = (int)m_fAtlasFrame;   // m_fAnimTime += dt * speed;
	if (frame >= 4)
	{
		frame = 3;             
	}

	int x = frame % 2;              // 2ї­
	int y = frame / 2;              // 2За
	m_AtlasIndex = { x * 0.5f, y * 0.5f };

	if (isEffectActive() == false)
		m_isAlive = false;
}

void CRagDolSmoke::Late_Update(_float dt)
{
}

void CRagDolSmoke::Render_GUI()
{
	
}

_bool CRagDolSmoke::isEffectActive()
{
	return m_fLifeTime < 1.f;
}

void CRagDolSmoke::Reset()
{
	m_pTransform->Scale({ 15, 15 , 15 });
	m_fLifeTime = 0.f;
}

void CRagDolSmoke::Set_DeActive()
{
	m_isAlive = false;
}

void CRagDolSmoke::Set_ReActive(const EffectData& data)
{
	Reset();
	m_isAlive = true;
}

CRagDolSmoke* CRagDolSmoke::Create()
{
	CRagDolSmoke* instance = new CRagDolSmoke();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CRagDolSmoke");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CRagDolSmoke::Clone(INIT_DESC* pArg)
{
	CRagDolSmoke* instance = new CRagDolSmoke(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CRagDolSmoke");
		Safe_Release(instance);
	}

	return instance;
}

void CRagDolSmoke::Free()
{
	__super::Free();
}
