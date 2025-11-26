#include "Client_Defines.h"
#include "CraftChildEffect.h"

#include "RectModel.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "Helper_Func.h"
#include "ObjectContainer.h"

CCraftChildEffect::CCraftChildEffect()
{
}

CCraftChildEffect::CCraftChildEffect(const CCraftChildEffect& rhs)
	: CBaseEffect(rhs)
{
}

HRESULT CCraftChildEffect::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CRectModel>();
	Add_Component<CMaterial>();

	return S_OK;
}

HRESULT CCraftChildEffect::Initialize(INIT_DESC* pArg)
{

	__super::Initialize(pArg);

	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();
	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	pRcsMgr->Add_ResourcePath("Effect_circle03.png", "../../Resources/Effect/circle03.png");
	pRcsMgr->Add_ResourcePath("Effect_circle05.png", "../../Resources/Effect/circle05.png");
	pRcsMgr->Add_ResourcePath("Effect_circle06.png", "../../Resources/Effect/circle06.png");

	CMaterial* pMaterial = Get_Component<CMaterial>();
	m_pMaterial_Instance = CMaterialInstance::Create_Handle("Rect_Effect_Base", "Particle", pDevice);

	pMaterial->Insert_MaterialInstance(m_pMaterial_Instance, nullptr);
	auto MaterialDat = m_pMaterial_Instance->Get_MaterialData();

	if (MaterialDat)
		MaterialDat->Link_Shader(G_GlobalLevelKey, "VTX_NorTexEffect.hlsl");

	m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_circle03.png", TEXTURE_TYPE::ALBEDO);
	m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_circle05.png", TEXTURE_TYPE::ALBEDO);
	m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_circle06.png", TEXTURE_TYPE::ALBEDO);

	m_pMaterial_Instance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
	m_pMaterial_Instance->Set_Param("fRadian", { &m_fRadian,"float",sizeof(float) });
	m_pMaterial_Instance->Set_Blended(true);

	m_pTransform->Scale({1.f,1.f,1.f});
	return S_OK;
}

void CCraftChildEffect::Priority_Update(_float dt)
{
}

void CCraftChildEffect::Update(_float dt)
{
	m_fLifeTime += dt;
	m_pTransform->Translate(XMLoadFloat3(&m_vMoveDir ) * dt);
	float damping = powf(0.95f, dt * 60.f);
	m_vMoveDir.x *= damping;
	m_vMoveDir.y *= damping;

	_float4 nowScale = {};
	XMStoreFloat4(&nowScale, m_pTransform->Get_Scale());

	if (nowScale.y > 1.f) {
		m_pTransform->AddScale({ 0,-m_vMoveDir.y * dt,0 });
	}
}

void CCraftChildEffect::Late_Update(_float dt)
{
}

void CCraftChildEffect::Render_GUI()
{
}

_bool CCraftChildEffect::isEffectActive()
{
	return (m_fDuration > m_fLifeTime) && (m_isAlive == true);
}

void CCraftChildEffect::Reset()
{
	m_fDuration = 0.f;
	m_fLifeTime = 0.f;
}

void CCraftChildEffect::Set_DeActive()
{
	m_isAlive = false;
}

void CCraftChildEffect::Set_Dir(_float3 dir, _float duration, _uint Index)
{
	Reset();
	m_isAlive = true;
	m_fDuration = duration;
	m_vMoveDir = dir;
	m_pMaterial_Instance->Get_TextureIndex()[0] = Index;

	m_fRadian = atan2f(m_vMoveDir.x, m_vMoveDir.y);
}

CCraftChildEffect* CCraftChildEffect::Create()
{
	CCraftChildEffect* instance = new CCraftChildEffect();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CCraftChildEffect");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CCraftChildEffect::Clone(INIT_DESC* pArg)
{
	CCraftChildEffect* instance = new CCraftChildEffect(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CCraftChildEffect");
		Safe_Release(instance);
	}

	return instance;
}

void CCraftChildEffect::Free()
{
	__super::Free();
}