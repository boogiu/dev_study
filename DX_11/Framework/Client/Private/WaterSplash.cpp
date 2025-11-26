#include "Client_Defines.h"
#include "WaterSplash.h"

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

CWaterSplash::CWaterSplash()
{
}

CWaterSplash::CWaterSplash(const CWaterSplash& rhs)
	:CBaseEffect(rhs)
{
}

HRESULT CWaterSplash::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
	Add_Component<CObjectContainer>();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CWaterSplash::Initialize(INIT_DESC * pArg)
{
	__super::Initialize(pArg);
	
	//m_pTransform->Rotate({ XMConvertToRadians(-90),0,0 });
	
	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	pRcsMgr->Add_ResourcePath("Effect_splash03_int.png", "../../Resources/Effect/splash03_int.png");

	Get_Component<CStaticModel>()->Link_Model("GamePlay_Level","watercolumn.model");
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "watercolumn.mat");

	for (auto& instance : Get_Component<CMaterial>()->Get_Material_Instance()) {
		instance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
		instance->Set_Param("DiffuseTexture", {pRcsMgr->Load_Texture("GamePlay_Level", "Effect_splash03_int.png")->Get_SRV(),"Texture2D", 0});
		instance->Override_Pass("Splash");
	}
	return S_OK;
}

void CWaterSplash::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CWaterSplash::Update(_float dt)
{
	m_fLifeTime += dt*3;
	m_pTransform->AddScale({ dt *30.5f,dt* 30.5f,dt * 30.5f });
	
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CWaterSplash::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CWaterSplash::Render_GUI()
{
}

_bool CWaterSplash::isEffectActive()
{
	return m_fLifeTime<1.f;
}

void CWaterSplash::Reset()
{
	m_fLifeTime = 0.f;
}

void CWaterSplash::Set_DeActive()
{
	m_isAlive = false;
	
}

void CWaterSplash::Set_ReActive(const EffectData& data)
{
	m_isAlive = true;
	Reset();
	m_pTransform->Scale({1.f,1.f,1.f});
}

CWaterSplash* CWaterSplash::Create()
{
	CWaterSplash* instance = new CWaterSplash();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CWaterSplash");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CWaterSplash::Clone(INIT_DESC* pArg)
{
	CWaterSplash* instance = new CWaterSplash(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CWaterSplash");
		Safe_Release(instance);
	}

	return instance;
}

void CWaterSplash::Free()
{
	__super::Free();
}
