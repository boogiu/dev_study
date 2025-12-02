#include "Client_Defines.h"
#include "FlowerEffect.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"
#include "InstanceModel.h"

#include "ObjectContainer.h"
#include "Helper_Func.h"
#include "Texture.h"
#include "AudioSource.h"
CFlowerEffect::CFlowerEffect()
{
}

CFlowerEffect::CFlowerEffect(const CFlowerEffect& rhs)
	:CBaseEffect(rhs)
{
}

HRESULT CFlowerEffect::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CInstanceModel>();
	Add_Component<CMaterial>();


	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "FieldPlant_FlowerSplash_00.wav", "Dash_Flower", false, SOUND_GROUP::SFX, 0.05f);
	return S_OK;
}

HRESULT CFlowerEffect::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	m_ParicleCount = 10;
	INSTANCE_INIT_DESC instanceDesc = {};
	instanceDesc.ElementKey = "ClientParticeInstance";
	instanceDesc.ElementCount = VTX_PARTICLEINSTANCE::iElementCount;
	instanceDesc.instanceStride = sizeof(INSTANCE_PARTICLE);
	instanceDesc.pElementDesc = VTX_PARTICLEINSTANCE::Elements;
	instanceDesc.instanceCount = m_ParicleCount;

	m_InitDescs.push_back(instanceDesc);
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "leaf.mat");
	Get_Component<CInstanceModel>()->Link_InstanceData(
		CGameInstance::GetInstance()->Get_Device(), m_InitDescs, "GamePlay_Level", "leaf.model");
	Get_Component<CInstanceModel>()->Link_InstanceMeshAll(0);
	Get_Component<CInstanceModel>()->ShadowCast(true);

	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	pRcsMgr->Add_ResourcePath("Effect_flwlily_alb.png", "../../Resources/Effect/flwlily_alb.png");
	pRcsMgr->Add_ResourcePath("Effect_flwlily_grd.png", "../../Resources/Effect/flwlily_grd.png");
	pRcsMgr->Add_ResourcePath("Effect_flwlily_nrm.png", "../../Resources/Effect/flwlily_nrm.png");
	pRcsMgr->Add_ResourcePath("Effect_gradation00.png", "../../Resources/Effect/gradation00.png");

	for (auto& instance : Get_Component<CMaterial>()->Get_Material_Instance()) {
		instance->Set_Param("GradationTexture", { pRcsMgr->Load_Texture("GamePlay_Level", "Effect_flwlily_grd.png")->Get_SRV(),"Texture2D", 0 });
		instance->Set_Param("AlbedoGrayTexture", { pRcsMgr->Load_Texture("GamePlay_Level","Effect_flwlily_alb.png")->Get_SRV(), "Texture2D",0 });
		instance->Set_Param("NormalTexture", { pRcsMgr->Load_Texture("GamePlay_Level","Effect_flwlily_nrm.png")->Get_SRV(), "Texture2D",0 });
		instance->Set_Param("MixtureTexture", { pRcsMgr->Load_Texture("GamePlay_Level", "Effect_gradation00.png")->Get_SRV(),"Texture2D", 0 });
		instance->Override_Pass("Particle_Leaf");
	}

	m_Particle.resize(m_ParicleCount, { {1,0,0,0},{0,1,0,0 },{0,0,1,0},{0,0,0,1},{0.3,0.4,0,1},{0,2} });
	m_vVelocities.resize(m_ParicleCount, { 0,0,0 });

	return S_OK;
}

void CFlowerEffect::Priority_Update(_float dt)
{

}

void CFlowerEffect::Update(_float dt)
{
	m_fLifeTime += dt;

	for (size_t i = 0; i < m_ParicleCount; i++)
	{
		m_vVelocities[i].y -=2.8f * dt;
		m_vVelocities[i].x -=2.8f*dt;
		m_vVelocities[i].z -=2.8f*dt;
		m_Particle[i].vLifetime.x += dt;

		_float vx = m_vVelocities[i].x * (cosf(m_Particle[i].vLifetime.x * 3));
		_float vz = m_vVelocities[i].z * (sinf(m_Particle[i].vLifetime.x * 3));
		m_Particle[i].vTranslation.x += vx*dt;
 		m_Particle[i].vTranslation.y += m_vVelocities[i].y * 10 * dt;
		m_Particle[i].vTranslation.z += vz*dt;
		m_Particle[i].vColor.z = atan2(vx, vz);
	}

	ID3D11DeviceContext* pContext = CGameInstance::GetInstance()->Get_Context();
	Get_Component<CInstanceModel>()->Update_Instance(pContext, m_Particle.data(), 0, static_cast<_uint>(m_Particle.size()));

}

void CFlowerEffect::Late_Update(_float dt)
{
}

void CFlowerEffect::Render_GUI()
{
}

_bool CFlowerEffect::isEffectActive()
{
	return m_fLifeTime < 3.f;
}

void CFlowerEffect::Reset()
{
	m_fLifeTime = 0.f;

	for (size_t i = 0; i < m_ParicleCount; i++)
	{
		m_Particle[i].vLifetime.y = Helper::Get_Random_Float(0.2, 0.8);

		_float angle = Helper::Get_Random_Float(0, XM_2PI);
		_float speed = Helper::Get_Random_Float(10.2f, 18.2f);
		_float4 pos = Get_Position();

		m_Particle[i].vTranslation.x = pos.x;
		m_Particle[i].vTranslation.y = pos.y;
		m_Particle[i].vTranslation.z = pos.z;

		m_vVelocities[i].x = cosf(angle) * speed;
		m_vVelocities[i].z = sinf(angle) * speed;
		m_vVelocities[i].y = Helper::Get_Random_Float(2.0f, 3.f);// À§·Î
		m_Particle[i].vLifetime.x = 0.f;
		m_Particle[i].vLifetime.y = Helper::Get_Random_Float(2.0f, 3.5f);
		m_Particle[i].vColor.x = Helper::Get_Random_Float(0.1f, 0.5f);
	}
}

void CFlowerEffect::Set_DeActive()
{
	m_isAlive = false;
}

void CFlowerEffect::Set_ReActive(const EffectData& data)
{
	Reset();
	m_isAlive = true;
	Get_Component<CAudioSource>()->Play("Dash_Flower");
}

CFlowerEffect* CFlowerEffect::Create()
{
	CFlowerEffect* instance = new CFlowerEffect();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CFlowerEffect");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CFlowerEffect::Clone(INIT_DESC* pArg)
{
	CFlowerEffect* instance = new CFlowerEffect(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CFlowerEffect");
		Safe_Release(instance);
	}

	return instance;
}

void CFlowerEffect::Free()
{
	__super::Free();
}
