#include "Client_Defines.h"
#include "WaterRing.h"

#include "RectModel.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "IRenderService.h"

CWaterRing::CWaterRing()
{
}

CWaterRing::CWaterRing(const CWaterRing& rhs)
	:CBaseEffect(rhs)
{
}

HRESULT CWaterRing::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CRectModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CWaterRing::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();
	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	IRenderService* pRenderSys = CGameInstance::GetInstance()->Get_RenderSystem();

	pRcsMgr->Add_ResourcePath("Effect_ring00.png", "../../Resources/Effect/ring00.png");
	pRcsMgr->Add_ResourcePath("Effect_ring01.png", "../../Resources/Effect/ring01.png");

	CMaterial* pMaterial = Get_Component<CMaterial>();
	CMaterialInstance* customInstance = CMaterialInstance::Create_Handle("Rect_Effect_Base", "Opaque", pDevice);

	pMaterial->Insert_MaterialInstance(customInstance, nullptr);
	auto MaterialDat = customInstance->Get_MaterialData();

	if (MaterialDat)
		MaterialDat->Link_Shader(G_GlobalLevelKey, "VTX_NorTexEffect.hlsl");

	customInstance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_ring00.png", TEXTURE_TYPE::ALBEDO);
	customInstance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_ring01.png", TEXTURE_TYPE::ALBEDO);
	customInstance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
	customInstance->Set_Param("fRadian", { &m_fRadian,"float",sizeof(float) });
	customInstance->Override_Pass("WaterPong");
	//customInstance->Set_Blended(true);
	m_pMaterial_Instance = customInstance;
	XMStoreFloat4(&m_vBasePos,m_pTransform->Get_Pos());
	return S_OK;
}

void CWaterRing::Priority_Update(_float dt)
{

}

void CWaterRing::Update(_float dt)
{
	m_fLifeTime += dt;
	_vector move = XMVector4Normalize(m_pTransform->Dir(STATE::UP ) - m_pTransform->Dir(STATE::RIGHT));
	m_pTransform->Translate(move * dt);
	m_pTransform->AddScale({dt,dt,dt});

	if (m_fLifeTime > 0.2f) {
		m_pMaterial_Instance->ChangeTexture(TEXTURE_TYPE::ALBEDO, 1);
	}
}

void CWaterRing::Late_Update(_float dt)
{
}

void CWaterRing::Render_GUI()
{
}

_bool CWaterRing::isEffectActive()
{
	return _bool();
}

void CWaterRing::Reset()
{
}

void CWaterRing::Set_DeActive()
{
	m_fLifeTime = 0.f;
	m_pMaterial_Instance->ChangeTexture(TEXTURE_TYPE::ALBEDO, 0);
	m_isAlive = false;
}

void CWaterRing::Set_ReActive(const EffectData& data)
{
	m_pTransform->Scale({ 1.f,1.f,1.f });
		m_isAlive = true;
	m_pTransform->Set_Pos(m_vBasePos);
}

CWaterRing* CWaterRing::Create()
{
	CWaterRing* instance = new CWaterRing();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CWaterRing");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CWaterRing::Clone(INIT_DESC* pArg)
{
	CWaterRing* instance = new CWaterRing(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CWaterPongEffect");
		Safe_Release(instance);
	}

	return instance;
}

void CWaterRing::Free()
{
	__super::Free();
}
