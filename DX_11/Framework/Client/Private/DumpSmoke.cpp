#include "Client_Defines.h"
#include "DumpSmoke.h"

#include "RectModel.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "ObjectContainer.h"
#include "Helper_Func.h"
#include "Texture.h"
#include "InstanceModel.h"

CDumpSmoke::CDumpSmoke()
{
}

CDumpSmoke::CDumpSmoke(const CDumpSmoke& rhs)
	: CBaseEffect(rhs)
{
}

HRESULT CDumpSmoke::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
	Add_Component<CInstanceModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CDumpSmoke::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	m_ParicleCount = 50;
	INSTANCE_INIT_DESC instanceDesc = {};
	instanceDesc.ElementKey = "ClientParticeInstance";
	instanceDesc.ElementCount = VTX_PARTICLEINSTANCE::iElementCount;
	instanceDesc.instanceStride = sizeof(INSTANCE_PARTICLE);
	instanceDesc.pElementDesc = VTX_PARTICLEINSTANCE::Elements;
	instanceDesc.instanceCount = m_ParicleCount;

	m_InitDescs.push_back(instanceDesc);
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "rubblestone.mat");
	Get_Component<CInstanceModel>()->Link_InstanceData(
		CGameInstance::GetInstance()->Get_Device(), m_InitDescs, "GamePlay_Level", "rubblestone.model");
	Get_Component<CInstanceModel>()->Link_InstanceMeshAll(0);


	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	pRcsMgr->Add_ResourcePath("Effect_smoke04.png", "../../Resources/Effect/smoke04.png");
	pRcsMgr->Add_ResourcePath("Effect_pltcolorgroundsoillump.png", "../../Resources/Effect/pltcolorgroundsoillump.png");

	for (auto& instance : Get_Component<CMaterial>()->Get_Material_Instance()) {
		instance->Set_Param("MixtureTexture", { pRcsMgr->Load_Texture("GamePlay_Level","Effect_smoke04.png")->Get_SRV(), "Texture2D",0});
		instance->Set_Param("DiffuseTexture", { pRcsMgr->Load_Texture("GamePlay_Level", "Effect_pltcolorgroundsoillump.png")->Get_SRV(),"Texture2D", 0 });
		instance->Override_Pass("Particle");
	}
	//smoke06_albgry.png
	m_Particle.resize(m_ParicleCount, { {1,0,0,0},{0,1,0,0 },{0,0,1,0},{0,0,0,1},{1,0,0,1},{0,5}});
	m_vVelocities.resize(m_ParicleCount, {0,0,0});


	return S_OK;
}

void CDumpSmoke::Priority_Update(_float dt)
{
}

void CDumpSmoke::Update(_float dt)
{
	m_fLifeTime += dt ;

	for (size_t i = 0; i < m_ParicleCount; i++)
	{
		m_vVelocities[i].y -= 9.8f * dt;

		m_Particle[i].vLifetime.x += dt;

		m_Particle[i].vTranslation.x += m_vVelocities[i].x*dt*1.5f;
		m_Particle[i].vTranslation.y += m_vVelocities[i].y*10*dt;
		m_Particle[i].vTranslation.z += m_vVelocities[i].z*dt * 1.5f;
	}

	ID3D11DeviceContext*pContext=	CGameInstance::GetInstance()->Get_Context();
	Get_Component<CInstanceModel>()->Update_Instance(pContext,m_Particle.data(), 0, static_cast<_uint>(m_Particle.size()));
}

void CDumpSmoke::Late_Update(_float dt)
{
}

void CDumpSmoke::Render_GUI()
{
	__super::Render_GUI();
}

_bool CDumpSmoke::isEffectActive()
{

	return m_fLifeTime < 1.f;
}

void CDumpSmoke::Reset()
{
	m_fLifeTime = 0.f;

	for (size_t i = 0; i < m_ParicleCount; i++)
	{
		m_Particle[i].vLifetime.y = Helper::Get_Random_Float(0.2, 0.8);

		_float angle = Helper::Get_Random_Float(0, XM_2PI);
		_float speed = Helper::Get_Random_Float(5.2f, 8.2f);

		m_Particle[i].vRight.x = Helper::Get_Random_Float(0.2, 0.6);
		m_Particle[i].vUp.y = Helper::Get_Random_Float(0.2, 0.6);
		m_Particle[i].vLook.z = Helper::Get_Random_Float(0.2, 0.6);

		m_Particle[i].vTranslation.x = 0.f;
		m_Particle[i].vTranslation.y = 0.f;
		m_Particle[i].vTranslation.z = 0.f;

		m_vVelocities[i].x = cosf(angle) * speed;
		m_vVelocities[i].z = sinf(angle) * speed;
		m_Particle[i].vLifetime.x = 0.f;
		m_vVelocities[i].y = Helper::Get_Random_Float(3.0f, 4.f);// À§·Î
	}
}

void CDumpSmoke::Set_DeActive()
{
	m_isAlive = false;
}

void CDumpSmoke::Set_ReActive(const EffectData& data)
{
	m_isAlive = true;
	Reset();
	m_pTransform->Scale({ 1.f,1.f,1.f });
}

CDumpSmoke* CDumpSmoke::Create()
{
	CDumpSmoke* instance = new CDumpSmoke();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CDumpSmoke");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CDumpSmoke::Clone(INIT_DESC* pArg)
{
	CDumpSmoke* instance = new CDumpSmoke(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CDumpSmoke");
		Safe_Release(instance);
	}

	return instance;
}

void CDumpSmoke::Free()
{
	__super::Free();
}
