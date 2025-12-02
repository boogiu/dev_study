#include "Client_Defines.h"
#include "LeafParticle.h"
#include "GameInstance.h"
#include "IResourceService.h"

#include "InstanceModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "Texture.h"
#include "Helper_Func.h"

CLeafParticle::CLeafParticle()
{
}

CLeafParticle::CLeafParticle(const CLeafParticle& rhs)
	:CGameObject(rhs)
{
}

HRESULT CLeafParticle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CInstanceModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CLeafParticle::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	m_ParticleCount = 20;
	INSTANCE_INIT_DESC instanceDesc = {};
	instanceDesc.ElementKey = "ClientMeshInstance";
	instanceDesc.ElementCount = VTX_PARTICLEINSTANCE::iElementCount;
	instanceDesc.instanceStride = sizeof(INSTANCE_PARTICLE);
	instanceDesc.pElementDesc = VTX_PARTICLEINSTANCE::Elements;
	instanceDesc.instanceCount = m_ParticleCount;
	m_InitDescs.push_back(instanceDesc);
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "leaf02.mat");
	Get_Component<CInstanceModel>()->Link_InstanceData(
		CGameInstance::GetInstance()->Get_Device(), m_InitDescs, "GamePlay_Level", "leaf02.model");
	Get_Component<CInstanceModel>()->Link_InstanceMeshAll(0);
	Get_Component<CInstanceModel>()->ShadowCast(true);

	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	pRcsMgr->Add_ResourcePath("Effect_treeoakleaf_alb.png", "../../Resources/Effect/treeoakleaf_alb.png");
	pRcsMgr->Add_ResourcePath("Effect_treeoakleaf_nrm.png", "../../Resources/Effect/treeoakleaf_nrm.png");
	pRcsMgr->Add_ResourcePath("Effect_treeoakleafcolor.png", "../../Resources/Effect/treeoakleafcolor.png");
	pRcsMgr->Add_ResourcePath("Effect_noiz02.png", "../../Resources/Effect/noiz02.png");

	for (auto& instance : Get_Component<CMaterial>()->Get_Material_Instance()) {
		instance->Set_Param("GradationTexture", { pRcsMgr->Load_Texture("GamePlay_Level", "Effect_treeoakleafcolor.png")->Get_SRV(),"Texture2D", 0 });
		instance->Set_Param("AlbedoGrayTexture", { pRcsMgr->Load_Texture("GamePlay_Level", "Effect_treeoakleaf_alb.png")->Get_SRV(),"Texture2D", 0 });
		instance->Set_Param("NormalTexture", { pRcsMgr->Load_Texture("GamePlay_Level", "Effect_treeoakleaf_nrm.png")->Get_SRV(),"Texture2D", 0 });
		instance->Set_Param("MixtureTexture", { pRcsMgr->Load_Texture("GamePlay_Level", "Effect_noiz02.png")->Get_SRV(),"Texture2D", 0 });
		instance->Override_Pass("Particle_Leaf");
	}

	m_Particle.resize(m_ParticleCount);
	m_vVelocity.resize(m_ParticleCount);

	for (size_t i = 0; i < m_ParticleCount; i++)
	{
		_float scale = Helper::Get_Random_Float(0.2, 1.f);
		m_Particle[i].vRight = { scale,0,0,0 };
		m_Particle[i].vUp = { 0,scale,0,0 };
		m_Particle[i].vLook = { 0,0,scale,0 };
		m_Particle[i].vColor = { 0.2,0.3,0,0 };

		m_vVelocity[i].x = Helper::Get_Random_Float(8, 10);
		m_vVelocity[i].y = Helper::Get_Random_Float(3, 8);
		m_vVelocity[i].z = Helper::Get_Random_Float(3, 8);
		m_Particle[i].vLifetime = { 0, Helper::Get_Random_Float(5,20) };
	}
	return S_OK;
}

void CLeafParticle::Priority_Update(_float dt)
{
}

void CLeafParticle::Update(_float dt)
{
	_float4 nowPos = Get_Position();
	for (size_t i = 0; i < m_ParticleCount; i++)
	{
		_float vx = cosf(m_Particle[i].vLifetime.x)	 + m_vVelocity[i].x;
		_float vy = sinf(m_Particle[i].vLifetime.x	*2.f) * 6 - m_vVelocity[i].y;
		m_Particle[i].vTranslation.x += vx *dt;
		m_Particle[i].vTranslation.y += dt * vy;
		m_Particle[i].vLifetime.x += dt;
		m_Particle[i].vColor.z = atan2(vx, -vy);

		if (m_Particle[i].vTranslation.y <= 1.f) {
			m_Particle[i].vTranslation.y = 1.f;
		}

		if (m_Particle[i].vLifetime.x >= m_Particle[i].vLifetime.y) {

			m_Particle[i].vTranslation = nowPos;
			m_Particle[i].vTranslation.y += 30;
			m_Particle[i].vTranslation.x += Helper::Get_Random_Float(-130, 100);
			m_Particle[i].vTranslation.z += Helper::Get_Random_Float(-150, 150);

			m_Particle[i].vLifetime = { 0, Helper::Get_Random_Float(5,20) };

			m_vVelocity[i].x = Helper::Get_Random_Float(5, 15);
			m_vVelocity[i].y = Helper::Get_Random_Float(2, 4);
		}
	}

	ID3D11DeviceContext* pContext = CGameInstance::GetInstance()->Get_Context();
	Get_Component<CInstanceModel>()->Update_Instance(pContext, m_Particle.data(), 0, m_ParticleCount);
}

void CLeafParticle::Late_Update(_float dt)
{
}

void CLeafParticle::Render_GUI()
{
}

CLeafParticle* CLeafParticle::Create()
{
	CLeafParticle* instance = new CLeafParticle();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CLeafParticle");
		Safe_Release(instance);
	}

	return instance;
}


CGameObject* CLeafParticle::Clone(INIT_DESC* pArg)
{
	CLeafParticle* instance = new CLeafParticle(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CLeafParticle");
		Safe_Release(instance);
	}

	return instance;
}

void CLeafParticle::Free()
{
	__super::Free();
}
