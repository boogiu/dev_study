#include "Client_Defines.h"
#include "EmotionEffect.h"
#include "RectModel.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "Helper_Func.h"

CEmotionEffect::CEmotionEffect()
{
}

CEmotionEffect::CEmotionEffect(const CEmotionEffect& rhs)
	: CBaseEffect(rhs)
{
}

HRESULT CEmotionEffect::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CRectModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CEmotionEffect::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();
	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	IRenderService* pRenderSys = CGameInstance::GetInstance()->Get_RenderSystem();

	pRcsMgr->Add_ResourcePath("Effect_noiz01.png", "../../Resources/Effect/noiz01.png");

	pRcsMgr->Add_ResourcePath("Effect_angrysmoke01.png", "../../Resources/Effect/angrysmoke01.png");
	pRcsMgr->Add_ResourcePath("Effect_brokenlovecrack.png", "../../Resources/Effect/brokenlovecrack.png");
	pRcsMgr->Add_ResourcePath("Effect_laugh05.png", "../../Resources/Effect/laugh05.png");
	pRcsMgr->Add_ResourcePath("Effect_questionmark02.png", "../../Resources/Effect/questionmark02.png");
	pRcsMgr->Add_ResourcePath("Effect_surprise01.png", "../../Resources/Effect/surprise01.png");
	pRcsMgr->Add_ResourcePath("Effect_surprise02.png", "../../Resources/Effect/surprise02.png");
	pRcsMgr->Add_ResourcePath("Effect_surprisemark03.png", "../../Resources/Effect/surprisemark03.png");
	pRcsMgr->Add_ResourcePath("Effect_sadmark06.png", "../../Resources/Effect/sadmark06.png");

	CMaterial* pMaterial = Get_Component<CMaterial>();
	CMaterialInstance* customInstance = CMaterialInstance::Create_Handle("Rect_Effect_Base", "Craft", pDevice);
	m_pMaterial_Instance = customInstance;

	/*0*/m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_angrysmoke01.png", TEXTURE_TYPE::ALBEDO);
	/*1*/m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_brokenlovecrack.png", TEXTURE_TYPE::ALBEDO);
	/*2*/m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_laugh05.png", TEXTURE_TYPE::ALBEDO);
	/*3*/m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_questionmark02.png", TEXTURE_TYPE::ALBEDO);
	/*4*/m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_surprise01.png", TEXTURE_TYPE::ALBEDO);
	/*5*/m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_surprise02.png", TEXTURE_TYPE::ALBEDO);
	/*6*/m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_surprisemark03.png", TEXTURE_TYPE::ALBEDO);
	/*7*/m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_sadmark06.png", TEXTURE_TYPE::ALBEDO);

	m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_noiz01.png", TEXTURE_TYPE::MIX);

	pMaterial->Insert_MaterialInstance(m_pMaterial_Instance, nullptr);
	auto MaterialDat = m_pMaterial_Instance->Get_MaterialData();

	if (MaterialDat)
		MaterialDat->Link_Shader(G_GlobalLevelKey, "VTX_NorTexEffect.hlsl");

	m_pMaterial_Instance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
	m_pMaterial_Instance->Set_Param("fRadian", { &m_fRadian,"float",sizeof(float) });
	m_pMaterial_Instance->Set_Param("DepthTexture", { pRenderSys->Get_EngineTargetSRV("Target_Depth"),"Texture2D", 0 });
	m_pMaterial_Instance->Set_Param("fAtlasIndex", { &m_AtlasIndex,"float2",sizeof(_float2) });
	m_pMaterial_Instance->Set_Param("fAtlasScale", { &m_AtlasScale,"float2",sizeof(_float2) });
	m_pMaterial_Instance->Override_Pass("Emotion");
	m_pMaterial_Instance->Set_Blended(true);

	return S_OK;
}

void CEmotionEffect::Priority_Update(_float dt)
{
	m_fLifeTime += dt;
}

void CEmotionEffect::Update(_float dt)
{
	m_pTransform->Scale_Vector(XMVectorLerp(m_pTransform->Get_Scale(), XMLoadFloat3(&m_vDstScale), dt*4));

	int columns = (int)m_AtlasCount.x;
	int rows = (int)m_AtlasCount.y;

	if (columns <= 0)
		return;

	m_fAtlasFrame += dt * 10;     // 속도 곱해주고 싶으면
	int frame = (int)m_fAtlasFrame;

	int totalFrames = (m_AtlasMaxFrame > 0)
		? m_AtlasMaxFrame
		: (columns * rows);

	if (totalFrames <= 0)
		return;

	if (frame >= totalFrames)
		frame = totalFrames - 1;

	int x = frame % columns;
	int y = frame / columns;

	m_AtlasIndex = { (float)x, (float)y };
}

void CEmotionEffect::Late_Update(_float dt)
{
}

void CEmotionEffect::Render_GUI()
{
}

_bool CEmotionEffect::isEffectActive()
{
	return m_fLifeTime < 2.f;
}

void CEmotionEffect::Reset()
{
	m_fLifeTime = 0.f;
	m_AtlasIndex = {};
	m_AtlasScale = {};
	m_AtlasCount = {};
	m_fAtlasFrame = 0;
	m_vDstScale = { 0,0,0 };
}

void CEmotionEffect::Set_DeActive()
{
	m_isAlive = false;
}

void CEmotionEffect::Set_ReActive(const EffectData& data)
{
	Reset();
	m_pTransform->Set_Pos({ data.FxPosition.x-5 ,data.FxPosition.y+15 ,data.FxPosition.z-5});
	Convert_ByData(data);
	m_isAlive = true;
}

void CEmotionEffect::Convert_ByData(const EffectData& data)
{
	
	_vector adjustY = { 0, 15,0,0 };
	_vector RqPos = XMLoadFloat4(&data.ReqPosition);
	_vector FXDir = XMLoadFloat4(&data.FxPosition)*2;
	_vector FXRight = XMVector3Cross({ 0,1,0,0 }, FXDir)*2;

	if (data.Additional == "Surprise") {
		m_AtlasScale = { 1 / 3.f,1 / 2.f };
		m_AtlasCount = { 3,2 };
		m_AtlasMaxFrame = 5;
		m_pMaterial_Instance->ChangeTexture(TEXTURE_TYPE::ALBEDO, 6);
		m_pTransform->Set_PosVector(RqPos-FXDir+FXRight+ adjustY);
		m_vDstScale = { 5,5,5 };
	}
	else if (data.Additional == "Surprise02") {
		m_AtlasScale = { 1 / 1.f,1 / 1.f };
		m_AtlasCount = { 1,1 };
		m_AtlasMaxFrame = 1;
		m_pMaterial_Instance->ChangeTexture(TEXTURE_TYPE::ALBEDO, 5);
		m_pTransform->Set_PosVector(RqPos-FXDir+FXRight+ adjustY);
		m_vDstScale = { 10,10,5 };
	}

	else if (data.Additional == "Question") {
		m_AtlasScale = { 1 / 3.f,1 / 4.f };
		m_AtlasCount = { 3,4 };
		m_AtlasMaxFrame = 12;
		m_pMaterial_Instance->ChangeTexture(TEXTURE_TYPE::ALBEDO, 3);
		m_pTransform->Set_PosVector(RqPos - FXDir + FXRight + adjustY);
		m_vDstScale = { 5,5,5 };
	}

	else if (data.Additional == "Sad") {
		m_AtlasScale = { 1 / 2.f,1 / 2.f };
		m_AtlasCount = { 2,2 };
		m_AtlasMaxFrame = 4;
		m_pMaterial_Instance->ChangeTexture(TEXTURE_TYPE::ALBEDO, 7);
		m_pTransform->Set_PosVector(RqPos - FXDir + FXRight + adjustY);
		m_vDstScale = { 5,5,5 };
	}

	else
	{
		m_isAlive = false;
	}
}
CEmotionEffect* CEmotionEffect::Create()
{
	CEmotionEffect* instance = new CEmotionEffect();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CEmotionEffect");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CEmotionEffect::Clone(INIT_DESC* pArg)
{
	CEmotionEffect* instance = new CEmotionEffect(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CEmotionEffect");
		Safe_Release(instance);
	}

	return instance;
}

void CEmotionEffect::Free()
{
	__super::Free();
}
