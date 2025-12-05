#include "Client_Defines.h"
#include "Stars.h"

#include "InstanceModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "GameInstance.h"
#include "Texture.h"
#include "Helper_Func.h"
CStars::CStars()
{
}

CStars::CStars(const CStars& rhs)
	:CGameObject(rhs)
{
}

HRESULT CStars::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CInstanceModel>();
	Add_Component<CMaterial>();

	return S_OK;
}

HRESULT CStars::Initialize(INIT_DESC* pArg)
{

	m_ParicleCount = 140;
	INSTANCE_INIT_DESC instanceDesc = {};
	instanceDesc.ElementKey = "ClientParticeInstance";
	instanceDesc.ElementCount = VTX_PARTICLEINSTANCE::iElementCount;
	instanceDesc.instanceStride = sizeof(INSTANCE_PARTICLE);
	instanceDesc.pElementDesc = VTX_PARTICLEINSTANCE::Elements;
	instanceDesc.instanceCount = m_ParicleCount;

	m_InitDescs.push_back(instanceDesc);
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "VRMoon.mat");
	Get_Component<CInstanceModel>()->Link_InstanceData(
		CGameInstance::GetInstance()->Get_Device(), m_InitDescs, "GamePlay_Level", "VRMoon.model");
	Get_Component<CInstanceModel>()->Link_InstanceMeshAll(0);


	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	pRcsMgr->Add_ResourcePath("Effect_mStar_OP.png", "../../Resources/Effect/mStar_OP.png");

	for (auto& instance : Get_Component<CMaterial>()->Get_Material_Instance()) {
		instance->Set_Param("MixtureTexture", { pRcsMgr->Load_Texture("GamePlay_Level","Effect_mStar_OP.png")->Get_SRV(), "Texture2D",0 });
		instance->Override_Pass("Stars");
	}

	m_Particles.resize(m_ParicleCount, { {10,0,0,0},{0,10,0,0 },{0,0,10,0},{0,0,0,1},{1,0,0,1},{0,5} });

	for (size_t i = 0; i < m_ParicleCount; i++)
	{
		m_Particles[i].vTranslation.x = Helper::Get_Random_Float(-650, 650);
		m_Particles[i].vTranslation.y = Helper::Get_Random_Float(200, 350);
		m_Particles[i].vTranslation.z = Helper::Get_Random_Float(-550, -300);
		m_Particles[i].vColor.x = Helper::Get_Random_Float(0, 0.8);
		m_Particles[i].vColor.y = Helper::Get_Random_Float(0, 0.8);
		m_Particles[i].vColor.z = Helper::Get_Random_Float(0.5, 1);
	}
	return S_OK;
}

void CStars::Priority_Update(_float dt)
{
}

void CStars::Update(_float dt)
{
	const _float4x4* pCamView = CGameInstance::GetInstance()->Get_CameraMgr()->Get_ViewMatrix();
	_matrix CamTransform = XMMatrixInverse(nullptr, XMLoadFloat4x4(pCamView));

	_vector camRight = XMVector3Normalize(CamTransform.r[0]);
	_vector camUp = XMVector3Normalize(CamTransform.r[1]);
	_vector camLook = XMVector3Normalize(CamTransform.r[2]);
	_vector camPos = CamTransform.r[3];   // 위치는 정규화 X

	_float dist = 10.f;

	_vector worldPos =camPos +camLook * dist;
	m_pTransform->Set_PosVector(worldPos);

	_vector up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_matrix viewBill = XMMatrixLookAtLH(worldPos, camPos, up);
	_matrix worldBill = XMMatrixInverse(nullptr, viewBill);

	m_pTransform->TranslateMatrix(worldBill);
	Update_Bounce(dt);


	ID3D11DeviceContext* pContext = CGameInstance::GetInstance()->Get_Context();
	Get_Component<CInstanceModel>()->Update_Instance(pContext, m_Particles.data(), 0, static_cast<_uint>(m_Particles.size()));
}

void CStars::Late_Update(_float dt)
{
}

void CStars::Render_GUI()
{
}

void CStars::Update_Bounce(_float dt)
{
	m_ElapsedTime += dt;
	const float period = 1.2f;

	_float localTime = fmodf(m_ElapsedTime, period);      // 0 ~ period
	_float phase = localTime / period;											// 0 ~ 1

	_float t = phase <= 0.5f ? phase * 2.0f : (1.0f - phase) * 2.0f;

	_float e = EaseIn(t);

	_float baseScale = 20.0f;
	_float bounceScale = 35.3f;
	_float scaleA = baseScale + (bounceScale - baseScale) * e;
	_float scaleB = baseScale + (bounceScale - baseScale) * (1 - e);

	for (size_t i = 0; i < m_ParicleCount/2; i++)
	{
		m_Particles[i].vRight.x = scaleA;
		m_Particles[i].vLook.z = scaleA;
		m_Particles[i].vUp.y = scaleA;
	}

	for (size_t i = m_ParicleCount / 2; i < m_ParicleCount; i++)
	{
		m_Particles[i].vRight.x = scaleB;
		m_Particles[i].vLook.z = scaleB;
		m_Particles[i].vUp.y = scaleB;
	}
}

_float CStars::EaseIn(_float time)
{
	_float c1 = 1.70158;
	_float c3 = c1 + 1;

	return c3 * time * time * time - c1 * time * time;
}

CStars* CStars::Create()
{
	CStars* instance = new CStars();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CStars");
		Safe_Release(instance);
	}

	return instance;
}


CGameObject* CStars::Clone(INIT_DESC* pArg)
{
	CStars* instance = new CStars(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CStars");
		Safe_Release(instance);
	}

	return instance;
}

void CStars::Free()
{
	__super::Free();
}
