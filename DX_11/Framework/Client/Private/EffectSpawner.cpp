#include "Client_Defines.h"
#include "EffectSpawner.h"
#include "BaseEffect.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "Helper_Func.h"
#include "DustEffect.h"

#include "CraftEffect.h"
#include "CraftChildEffect.h"
#include "CraftTwinkle.h"

#include "WaterPongEffect.h"
#include "WaterRing.h"
#include "WaterSplash.h"
#include "WaterWaveSplash.h"

#include "RagDolStone.h"
#include "RagDolPart.h"
#include "RagDolSmoke.h"

#include "GroundDump.h"
#include "DumpParts.h"
#include "DumpSmoke.h"

#include "BurryingGround.h"
#include "GroundPart.h"

#include "SmokeEffect.h"
#include "FlowerEffect.h"
#include "EmotionEffect.h"

#include "NoteEffect.h"
CEffectSpawner::CEffectSpawner()
{
}

CEffectSpawner::CEffectSpawner(const CEffectSpawner& rhs)
	:CLevelObject(rhs)
{
}

HRESULT CEffectSpawner::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	auto pProtoMgr = CGameInstance::GetInstance()->Get_PrototypeMgr();
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_BaseEffect", CBaseEffect::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_DustEffect", CDustEffect::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_CraftEffect", CCraftEffect::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_CraftChildEffect", CCraftChildEffect::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_CraftTwinkle", CCraftTwinkle::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_WaterPong", CWaterPongEffect::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_WaterRing", CWaterRing::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_WaterSplash", CWaterSplash::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_WaterWaveSplash", CWaterWaveSplash::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_RagDolStone", CRagDolStone::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_RagDolPart", CRagDolPart::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_RagDolSmoke", CRagDolSmoke::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_GroundDump", CGroundDump::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_DumpParts", CDumpParts::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_DumpSmoke", CDumpSmoke::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_BurryingGround", CBurryingGround::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_GroundPart", CGroundPart::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_SmokeEffect", CSmokeEffect::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_FlowerEffect", CFlowerEffect::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_EmotionEffect", CEmotionEffect::Create());
	pProtoMgr->Add_ProtoType("GamePlay_Level", "GamePlay_Effect_NoteEffect", CNoteEffect::Create());

	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	pRcsMgr->Add_ResourcePath("VTX_NorTexEffect.hlsl", "../Bin/ShaderFiles/VTX_NorTexEffect.hlsl");

	return S_OK;
}

HRESULT CEffectSpawner::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CEffectSpawner::Priority_Update(_float dt)
{
}

void CEffectSpawner::Update(_float dt)
{
	for (auto& Effect : m_ActivePool)
	{
		if (!Effect->isEffectActive())
			m_ReturnPool.push_back(Effect);
	}
}

void CEffectSpawner::Late_Update(_float dt)
{
	for (auto& Effect : m_ReturnPool)
	{
		Effect->Set_DeActive();
		m_ActivePool.erase(std::find(m_ActivePool.begin(), m_ActivePool.end(), Effect));
		Effect->Reset();
		m_EffectPool[Effect->Get_Tag()].push_back(Effect);
	}
	m_ReturnPool.clear();
}

void CEffectSpawner::Request_Effect(const string& tag, const EffectData& data)
{
	CBaseEffect* effect = nullptr;

	auto& pool = m_EffectPool[tag];
	if (!pool.empty()) {
		effect = pool.back();
		pool.pop_back();
	}
	else {
		effect = Build_Effect(tag); // Clone 매커니즘
	}
	effect->Set_Tag(tag);
	effect->Get_Component<CTransform>()->Set_Pos(data.FxPosition);
	effect->Set_ReActive(data);
	m_ActivePool.push_back(effect);
}

CBaseEffect* CEffectSpawner::Build_Effect(const string& tag)
{
	CGameObject* pEffect = { nullptr };

	if (tag == "Effect_Dust") {
		pEffect = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_DustEffect" }).Build("Effect_Dust");
	}
	if (tag == "Effect_Craft") {
		pEffect = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_CraftEffect" }).Build("Effect_Craft");
	}
	if (tag == "Effect_WaterPong") {
		pEffect = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_WaterPong" }).Build("Effect_WaterPong");
	}
	if (tag == "Effect_WaterSplash") {
		pEffect = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_WaterSplash" }).Build("Effect_WaterSplash");
	}
	if (tag == "Effect_WaterWaveSplash") {
		pEffect = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_WaterWaveSplash" }).Build("Effect_WaterWaveSplash");
	}
	if (tag == "Effect_RagDolStone") {
		pEffect = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_RagDolStone" }).Build("Effect_RagDolStone");
	}
	if (tag == "Effect_GroundDump") {
		pEffect = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_GroundDump" }).Build("Effect_GroundDump");
	}
	if (tag == "Effect_BurryingGround") {
		pEffect = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_BurryingGround" }).Build("Effect_BurryingGround");
	}
	if (tag == "Effect_Smoke") {
		pEffect = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_SmokeEffect" }).Build("Effect_Smoke");
	}
	if (tag == "Effect_Flower") {
		pEffect = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_FlowerEffect" }).Build("Effect_Flower");
	}
	if (tag == "Effect_Emotion") {
		pEffect = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_EmotionEffect" }).Build("Effect_Emotion");
	}
	if (tag == "Effect_Note") {
		pEffect = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_NoteEffect" }).Build("Effect_Note");
	}
	CGameInstance::GetInstance()->Get_ObjectMgr()->Add_Object(pEffect, { "GamePlay_Level" , "Effect_Layer" });

	return dynamic_cast<CBaseEffect*>(pEffect);
}

CEffectSpawner* CEffectSpawner::Create()
{
	CEffectSpawner* instance = new CEffectSpawner();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CEffectSpawner");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CEffectSpawner::Clone(INIT_DESC* pArg)
{
	CEffectSpawner* instance = new CEffectSpawner(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CEffectSpawner");
		Safe_Release(instance);
	}

	return instance;
}

void CEffectSpawner::Free()
{
	__super::Free();
}