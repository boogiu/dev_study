#include "Client_Defines.h"
#include "WaterPongEffect.h"

#include "RectModel.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "ObjectContainer.h"
#include "StaticModel.h"

#include "IRenderService.h"
#include "Texture.h"

CWaterPongEffect::CWaterPongEffect()
{
}

CWaterPongEffect::CWaterPongEffect(const CWaterPongEffect& rhs)
	:CBaseEffect(rhs)
{
}

HRESULT CWaterPongEffect::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
	Add_Component<CObjectContainer>();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CWaterPongEffect::Initialize(INIT_DESC * pArg)
{
	__super::Initialize(pArg);

//CGameObject* pRing1 = 
//	Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_WaterRing" }).Position({ 0,3,0}).Rotate({ XMConvertToRadians(90),0,0}).Build("Ring");
//CGameObject* pRing2 =
//	Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_WaterRing" }).Position({ 0,3,0 }).Rotate({ XMConvertToRadians(90),XMConvertToRadians(90),0}).Build("Ring");
//CGameObject* pRing3 = 
//	Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_WaterRing" }).Position({ 0,3,0 }).Rotate({ XMConvertToRadians(90),XMConvertToRadians(180),0}).Build("Ring");
//CGameObject* pRing4 =
//	Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_WaterRing" }).Position({ 0,3,0 }).Rotate({ XMConvertToRadians(90),XMConvertToRadians(270),0}).Build("Ring");
//
//Get_Component<CObjectContainer>()->Add_Child(pRing1, true);
//Get_Component<CObjectContainer>()->Add_Child(pRing2, true);
//Get_Component<CObjectContainer>()->Add_Child(pRing3, true);
//Get_Component<CObjectContainer>()->Add_Child(pRing4, true);
//
//m_Rings.push_back(dynamic_cast<CBaseEffect*>(pRing1));
//m_Rings.push_back(dynamic_cast<CBaseEffect*>(pRing2));
//m_Rings.push_back(dynamic_cast<CBaseEffect*>(pRing3));
//m_Rings.push_back(dynamic_cast<CBaseEffect*>(pRing4));
	
	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	pRcsMgr->Add_ResourcePath("Effect_warter00.png", "../../Resources/Effect/warter00.png");

	Get_Component<CStaticModel>()->Link_Model("GamePlay_Level","ripple02.model");
	//Get_Component<CStaticModel>()->ShadowCast(true);
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "ripple02.mat");
	for (auto& instance : Get_Component<CMaterial>()->Get_Material_Instance()) {
		instance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
		instance->Set_Param("DiffuseTexture", {pRcsMgr->Load_Texture("GamePlay_Level", "Effect_warter00.png")->Get_SRV(),"Texture2D", 0});
	}
	return S_OK;
}

void CWaterPongEffect::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CWaterPongEffect::Update(_float dt)
{
	m_fLifeTime += dt;
	m_pTransform->AddScale({ dt * 4.5f,dt * 1.5f,dt * 4.5f });
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CWaterPongEffect::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CWaterPongEffect::Render_GUI()
{
}

_bool CWaterPongEffect::isEffectActive()
{
	return m_fLifeTime<1.f;
}

void CWaterPongEffect::Reset()
{
	//		for (auto& Ring : m_Rings)
	//		{
	//			Ring->Reset();
	//		}
	m_fLifeTime = 0.f;
}

void CWaterPongEffect::Set_DeActive()
{
	m_isAlive = false;
	//	for (auto& Ring : m_Rings)
	//	{
	//		Ring->Set_DeActive();
	//	}
}

void CWaterPongEffect::Set_ReActive(const EffectData& data)
{
	m_isAlive = true;
	Reset();
	m_pTransform->Scale({1.f,1.f,1.f});
	//	for (auto& Ring : m_Rings)
	//	{
	//		Ring->Set_ReActive(data);
	//	}
}

CWaterPongEffect* CWaterPongEffect::Create()
{
	CWaterPongEffect* instance = new CWaterPongEffect();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CWaterPongEffect");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CWaterPongEffect::Clone(INIT_DESC* pArg)
{
	CWaterPongEffect* instance = new CWaterPongEffect(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CWaterPongEffect");
		Safe_Release(instance);
	}

	return instance;
}

void CWaterPongEffect::Free()
{
	__super::Free();
}
