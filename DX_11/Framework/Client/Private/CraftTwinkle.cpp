#include "Client_Defines.h"
#include "CraftTwinkle.h"

#include "RectModel.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "Helper_Func.h"

CCraftTwinkle::CCraftTwinkle()
{
}

CCraftTwinkle::CCraftTwinkle(const CCraftTwinkle& rhs)
    : CBaseEffect(rhs)
{

}

HRESULT CCraftTwinkle::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    Add_Component<CRectModel>();
    Add_Component<CMaterial>();
    return S_OK;
}

HRESULT CCraftTwinkle::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();
	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	pRcsMgr->Add_ResourcePath("Effect_twink04.png", "../../Resources/Effect/twink04.png");

	CMaterial* pMaterial = Get_Component<CMaterial>();
	CMaterialInstance* customInstance = CMaterialInstance::Create_Handle("Rect_Effect_Base", "Craft", pDevice);

	pMaterial->Insert_MaterialInstance(customInstance, nullptr);
	auto MaterialDat = customInstance->Get_MaterialData();

	if (MaterialDat)
		MaterialDat->Link_Shader(G_GlobalLevelKey, "VTX_NorTexEffect.hlsl");

	customInstance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_twink04.png", TEXTURE_TYPE::ALBEDO);
	customInstance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
	customInstance->Set_Param("fRadian", { &m_fRadian,"float",sizeof(float) });

	customInstance->Set_Param("fAtlasIndex", { &m_AtlasIndex,"float2",sizeof(_float2) });
	customInstance->Set_Param("fAtlasScale", { &m_AtlasScale,"float2",sizeof(_float2) });
	customInstance->Set_Blended(true);

	m_pMaterial_Instance = customInstance;
	m_AtlasScale = { 1 / 3.f,1 / 2.f };

    return S_OK;
}

void CCraftTwinkle::Priority_Update(_float dt)
{
}

void CCraftTwinkle::Update(_float dt)
{

	m_fAtlasFrame += dt * m_fFrameTime;

	int frame = (int)m_fAtlasFrame;   // m_fAnimTime += dt * speed;

	int x = frame % 3;              // 2ї­
	int y = frame / 1;              // 2За

	m_AtlasIndex = { (float)x, (float)y };

}

void CCraftTwinkle::Late_Update(_float dt)
{
}

void CCraftTwinkle::Render_GUI()
{
}

_bool CCraftTwinkle::isEffectActive()
{
	return (m_fLifeTime < 3.f) && m_isAlive;
}

void CCraftTwinkle::Reset()
{
	m_pTransform->Scale({ 1, 1 , 1 });
	m_fLifeTime = 0.f;
	m_fRadian = 0.f;
	m_fAtlasFrame = 0.f;
	m_AtlasIndex = {};
}

void CCraftTwinkle::Set_DeActive()
{
	m_isAlive = false;
}

void CCraftTwinkle::Set_ReActive(const EffectData& data)
{
	Reset();
	m_isAlive = true;
	m_fFrameTime = Helper::Get_Random_Float(5, 10);
}

CCraftTwinkle* CCraftTwinkle::Create()
{
	CCraftTwinkle* instance = new CCraftTwinkle();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CCraftTwinkle");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CCraftTwinkle::Clone(INIT_DESC* pArg)
{
	CCraftTwinkle* instance = new CCraftTwinkle(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CCraftTwinkle");
		Safe_Release(instance);
	}

	return instance;
}

void CCraftTwinkle::Free()
{
	__super::Free();
}
