#include "Client_Defines.h"
#include "DustEffect.h"
#include "RectModel.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

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
	pRcsMgr->Add_ResourcePath("VTX_NorTexEffect.hlsl", "../Bin/ShaderFiles/VTX_NorTexEffect.hlsl");
	pRcsMgr->Add_ResourcePath("Effect_smoke05.png", "../../Resources/Effect/smoke05.png");
	pRcsMgr->Add_ResourcePath("Effect_smoke06_albgry.png", "../../Resources/Effect/smoke06_albgry.png");

	CMaterial* pMaterial = Get_Component<CMaterial>();
	CMaterialInstance* customInstance = CMaterialInstance::Create_Handle("Rect_Effect_Base", "Opaque", pDevice);

	pMaterial->Insert_MaterialInstance(customInstance, nullptr);
	auto MaterialDat = customInstance->Get_MaterialData();

	if (MaterialDat)
		MaterialDat->Link_Shader(G_GlobalLevelKey, "VTX_NorTexEffect.hlsl");

	customInstance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_smoke05.png", TEXTURE_TYPE::ALBEDO);
	customInstance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_smoke06_albgry.png", TEXTURE_TYPE::ALBEDO_GRAY);
	customInstance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
	m_pMaterial_Instance = customInstance;

	return S_OK;
}

void CDustEffect::Priority_Update(_float dt)
{
}

void CDustEffect::Update(_float dt)
{
	m_fLifeTime += dt *2;
	if (m_fLifeTime > 1.f)
		m_fLifeTime = 0.f;
	m_pTransform->Scale({ m_fLifeTime *5,m_fLifeTime * 5 ,m_fLifeTime * 5 });
	m_pTransform->Rotate({ 0,0,m_fLifeTime *2 });
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
