#include "Client_Defines.h"
#include "CraftEffect.h"
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
#include "AudioSource.h"
CCraftEffect::CCraftEffect()
{
}

CCraftEffect::CCraftEffect(const CCraftEffect& rhs)
	: CBaseEffect(rhs)
{
}

HRESULT CCraftEffect::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CRectModel>();
	Add_Component<CMaterial>();
	Add_Component<CObjectContainer>();
	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "Demo_WorkBench_Finish.wav", "Craft_Finish", false, SOUND_GROUP::SFX, 0.15f);

	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "Demo_WorkBench_Cut_Wood.wav", "CraftSound_1", true, SOUND_GROUP::SFX, 0.25f);
	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "Demo_WorkBench_Hit_Wood_1.wav", "CraftSound_2", false, SOUND_GROUP::SFX, 0.25f);
	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "Demo_WorkBench_Making_Back.wav", "CraftSound_3", true, SOUND_GROUP::SFX, 0.25f);
	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "Demo_WorkBench_Rotate_Wood.wav", "CraftSound_4", true, SOUND_GROUP::SFX, 0.25f);
	return S_OK;
}

HRESULT CCraftEffect::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();
	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	IRenderService* pRenderSys = CGameInstance::GetInstance()->Get_RenderSystem();

	pRcsMgr->Add_ResourcePath("Effect_absentmindcloudblur.png", "../../Resources/Effect/absentmindcloudblur.png");

	CMaterial* pMaterial = Get_Component<CMaterial>();
	CMaterialInstance* customInstance = CMaterialInstance::Create_Handle("Rect_Effect_Base", "Craft", pDevice);

	pMaterial->Insert_MaterialInstance(customInstance, nullptr);
	auto MaterialDat = customInstance->Get_MaterialData();

	if (MaterialDat)
		MaterialDat->Link_Shader(G_GlobalLevelKey, "VTX_NorTexEffect.hlsl");

	customInstance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_absentmindcloudblur.png", TEXTURE_TYPE::ALBEDO);
	customInstance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
	customInstance->Set_Param("fRadian", { &m_fRadian,"float",sizeof(float) });
	customInstance->Set_Param("DepthTexture", { pRenderSys->Get_EngineTargetSRV("Target_Depth"),"Texture2D", 0 });
	customInstance->Set_Param("fAtlasIndex", { &m_AtlasIndex,"float2",sizeof(_float2) });
	customInstance->Set_Param("fAtlasScale", { &m_AtlasScale,"float2",sizeof(_float2) });
	customInstance->Set_Blended(true);

	m_pMaterial_Instance = customInstance;
	m_AtlasScale = { 1 / 2.f,1 / 2.f };

	for (size_t i = 0; i < 30; i++)
	{
		CGameObject* pObj = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_CraftChildEffect" }).Build("Child");
		Get_Component<CObjectContainer>()->Add_Child(pObj, false);
		m_Childs.push_back(dynamic_cast<CCraftChildEffect*>(pObj));
	}

	for (size_t i = 0; i < 10; i++)
	{
		CGameObject* pObj = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_CraftTwinkle" }).Build("Twinkle");
		Get_Component<CObjectContainer>()->Add_Child(pObj, false);
		m_Twinckles.push_back(dynamic_cast<CCraftTwinkle*>(pObj));
	}

	for (auto child : m_Childs)
	{
		child->Set_DeActive();
	}
	for (auto twinkle : m_Twinckles)
	{
		twinkle->Set_DeActive();
	}
	return S_OK;
}

void CCraftEffect::Priority_Update(_float dt)
{
	for (auto child : m_Childs)
	{
		if (!child->isEffectActive()) {
			child->Set_DeActive();
		}
	}
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);

}

void CCraftEffect::Update(_float dt)
{
	m_fLifeTime += dt;

	float size = cosf(m_fLifeTime * 5) * 0.5f;
	m_pTransform->AddScale({ size,size  ,0.f });

	m_fAtlasFrame += dt * 8;
	int frame = (int)m_fAtlasFrame;   // m_fAnimTime += dt * speed;

	int x = frame % 2;              // 2ї­
	int y = frame / 2;              // 2За

	m_AtlasIndex = { (float)x, (float)y };
	Get_Component<CObjectContainer>()->UpdateChild(dt);
	Get_Component<CAudioSource>()->Play("CraftSound_1");
	Get_Component<CAudioSource>()->Play("CraftSound_4");

	if (m_fLifeTime > 0.2f && m_eState == phase1) {
		Emit_Particle(5);
		m_eState = phase2;
		Get_Component<CAudioSource>()->Play("CraftSound_3");
		Get_Component<CAudioSource>()->Play("CraftSound_2");
	}
	if (m_fLifeTime > 0.8f && m_eState == phase2) {
		Emit_Particle(5);
		m_eState = phase3;
		Get_Component<CAudioSource>()->Play("CraftSound_3");
		Get_Component<CAudioSource>()->Play("CraftSound_2");
	}
	if (m_fLifeTime > 1.8f && m_eState == phase3) {
		Emit_Particle(5);
		m_eState = lastPhase;
		Get_Component<CAudioSource>()->Play("CraftSound_3");
		Get_Component<CAudioSource>()->Play("CraftSound_2");
	}
	if (m_fLifeTime > 2.6f && m_eState == lastPhase) {
		Emit_Particle(15);
		Emit_Twinkle();
		m_eState = End;
		Get_Component<CAudioSource>()->Play("Craft_Finish");
	}
	if (m_eState == End)
		m_pTransform->AddScale({ -dt * 8,-dt * 8,-dt });
}

void CCraftEffect::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CCraftEffect::Render_GUI()
{
	if (ImGui::Button("Emit")) {
		Emit_Particle(3);
	}
}

_bool CCraftEffect::isEffectActive()
{
	return m_fLifeTime < 3.4f;
}

void CCraftEffect::Reset()
{
	m_pTransform->Scale({ 15, 15 , 15 });
	m_fLifeTime = 0.f;
}

void CCraftEffect::Set_DeActive()
{
	m_isAlive = false;

	for (auto child : m_Childs)
	{
		child->Set_DeActive();
	}
	for (auto twinkle : m_Twinckles)
	{
		twinkle->Set_DeActive();
	}
}

void CCraftEffect::Set_ReActive(const EffectData& data)
{
	Reset();
	m_isAlive = true;
	for (auto child : m_Childs)
		child->Get_Component<CTransform>()->Set_PosVector(m_pTransform->Get_Pos());
}

void CCraftEffect::Emit_Particle(_float count, _bool isEnd)
{
	_uint emitCount = {};
	for (auto child : m_Childs)
	{
		if (!child->isEffectActive()) {
			child->Set_ReActive({});
			_float fLifeTime = Helper::Get_Random_Float(0.4f, 0.9f);
			_float X = Helper::Get_Random_Float(-15, 15) * 3;
			_float Y;
			_float scaleY = Helper::Get_Random_Float(1, 3);

			if (isEnd) {
				X *= 5;
				Y = Helper::Get_Random_Float(18, 25) * 3;
			}
			else {
				Y = Helper::Get_Random_Float(8, 13) * 3;
			}
			_float index = Helper::Get_Random_Int(0, 2);
			child->Set_Dir({ X,Y,0 }, fLifeTime, index);
			emitCount++;
			child->Get_Component<CTransform>()->Set_PosVector(m_pTransform->Get_Pos());
			child->Get_Component<CTransform>()->Scale({ 1,Y*0.3f,1.f });
		}

		if (emitCount == count)
			break;
	}
}

void CCraftEffect::Emit_Twinkle()
{
	for (auto twinkle : m_Twinckles)
	{
		if (!twinkle->isEffectActive()) {
			twinkle->Set_ReActive({});
			_float X = Helper::Get_Random_Float(-15, 15);
			_float Y = Helper::Get_Random_Float(-8, 13);
			_float scale = Helper::Get_Random_Float(0.2, 1.5f);
			_float4 pos = Get_Position();
			twinkle->Get_Component<CTransform>()->Set_Pos({ pos.x + X,pos.y + Y,pos.z });
			twinkle->Get_Component<CTransform>()->Scale({ scale,scale,1.f });
		}
	}
}

CCraftEffect* CCraftEffect::Create()
{
	CCraftEffect* instance = new CCraftEffect();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CCraftEffect");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CCraftEffect::Clone(INIT_DESC* pArg)
{
	CCraftEffect* instance = new CCraftEffect(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CCraftEffect");
		Safe_Release(instance);
	}

	return instance;
}

void CCraftEffect::Free()
{
	__super::Free();
}
