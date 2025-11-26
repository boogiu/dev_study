#include "Client_Defines.h"
#include "WaterWaveSplash.h"

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

CWaterWaveSplash::CWaterWaveSplash()
{
}

CWaterWaveSplash::CWaterWaveSplash(const CWaterWaveSplash& rhs)
	:CBaseEffect(rhs)
{
}

HRESULT CWaterWaveSplash::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
	Add_Component<CObjectContainer>();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CWaterWaveSplash::Initialize(INIT_DESC * pArg)
{
	__super::Initialize(pArg);
	
	//m_pTransform->Rotate({ XMConvertToRadians(-90),0,0 });
	
	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	pRcsMgr->Add_ResourcePath("Effect_splash01_int.png", "../../Resources/Effect/splash01_int.png");

	Get_Component<CStaticModel>()->Link_Model("GamePlay_Level","ripple02.model");
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "ripple02.mat");

	for (auto& instance : Get_Component<CMaterial>()->Get_Material_Instance()) {
		instance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
		instance->Set_Param("DiffuseTexture", {pRcsMgr->Load_Texture("GamePlay_Level", "Effect_splash01_int.png")->Get_SRV(),"Texture2D", 0});
		instance->Override_Pass("Splash");
	}
	return S_OK;
}

void CWaterWaveSplash::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CWaterWaveSplash::Update(_float dt)
{
	m_fLifeTime += dt;
	m_pTransform->AddScale({ dt *10.5f,0,dt * 10.5f });
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CWaterWaveSplash::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CWaterWaveSplash::Render_GUI()
{
}

_bool CWaterWaveSplash::isEffectActive()
{
	return m_fLifeTime<1.f;
}

void CWaterWaveSplash::Reset()
{
	m_fLifeTime = 0.f;
}

void CWaterWaveSplash::Set_DeActive()
{
	m_isAlive = false;
	
}

void CWaterWaveSplash::Set_ReActive(const EffectData& data)
{
	m_isAlive = true;
	Reset();
	m_pTransform->Scale({1.f,1.f,1.f});
}

CWaterWaveSplash* CWaterWaveSplash::Create()
{
	CWaterWaveSplash* instance = new CWaterWaveSplash();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CWaterWaveSplash");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CWaterWaveSplash::Clone(INIT_DESC* pArg)
{
	CWaterWaveSplash* instance = new CWaterWaveSplash(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CWaterWaveSplash");
		Safe_Release(instance);
	}

	return instance;
}

void CWaterWaveSplash::Free()
{
	__super::Free();
}
