#include "Client_Defines.h"
#include "Moon.h"
#include "StaticModel.h"
#include "Material.h"
#include "GameInstance.h"
CMoon::CMoon()
{
}

CMoon::CMoon(const CMoon& rhs)
	:CGameObject(rhs)
{
}

HRESULT CMoon::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CMoon::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Get_Component<CStaticModel>()->Link_Model("GamePlay_Level","VRMoon.model");
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level","VRMoon.mat");
	m_pTransform->Scale({ 2.8f,2.8f,2.8f });
	return S_OK;
}

void CMoon::Priority_Update(_float dt)
{
	const _float4x4* pCamView = CGameInstance::GetInstance()->Get_CameraMgr()->Get_ViewMatrix();
	_matrix CamTransform = XMMatrixInverse(nullptr, XMLoadFloat4x4(pCamView));

	_vector camRight = XMVector3Normalize(CamTransform.r[0]);
	_vector camUp = XMVector3Normalize(CamTransform.r[1]);
	_vector camLook = XMVector3Normalize(CamTransform.r[2]);
	_vector camPos = CamTransform.r[3];   // 위치는 정규화 X

	_float dist = 7.f;
	_float offsetX = 4.5f;
	_float offsetY = 2.5f;

	_vector worldPos =
		camPos +
		camRight * offsetX +
		camUp * offsetY +
		camLook * dist;

	m_pTransform->Set_PosVector(worldPos);
	_vector up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_matrix viewBill = XMMatrixLookAtLH(worldPos, camPos, up);
	_matrix worldBill = XMMatrixInverse(nullptr, viewBill);
	m_pTransform->TranslateMatrix(worldBill);
	m_pTransform->Scale({ 2.8f,2.8f,2.8f });

	//Update_Bounce(dt);
}


void CMoon::Update(_float dt)
{
}

void CMoon::Late_Update(_float dt)
{
}
void CMoon::Render_GUI()
{
	__super::Render_GUI();
	//		if (ImGui::Begin("Moon Debug"))
	//		{
	//			ImGui::Text("Screen-space Moon");
	//		
	//			ImGui::SliderFloat("Distance", &dist, 1.f, 200.f);   // 카메라로부터 거리
	//			ImGui::SliderFloat("Offset X", &offsetX, -100.f, 100.f); // 오른쪽/왼쪽
	//			ImGui::SliderFloat("Offset Y", &offsetY, -100.f, 100.f); // 위/아래
	//		}
	//		ImGui::End();
}

void CMoon::Update_Bounce(_float dt)
{
	m_ElapsedTime += dt;
	const float period = 0.6f;

	_float localTime = fmodf(m_ElapsedTime, period);      // 0 ~ period
	_float phase = localTime / period;											// 0 ~ 1

	_float t = phase <= 0.5f ? phase * 2.0f : (1.0f - phase) * 2.0f;

	_float e = EaseIn(t);
	
	_float baseScale = 1.0f;
	_float bounceScale = 1.1f;
	_float scale = baseScale + (bounceScale - baseScale) * e;
	m_pTransform->Translate({ scale, scale, scale });
}

_float CMoon::EaseIn(_float time)
{
	_float c1 = 1.70158;
	_float c3 = c1 + 1;

	return c3 * time * time * time - c1 * time * time;
}

CMoon* CMoon::Create()
{
	CMoon* instance = new CMoon();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CMoon");
		Safe_Release(instance);
	}

	return instance;
}


CGameObject* CMoon::Clone(INIT_DESC* pArg)
{
	CMoon* instance = new CMoon(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CMoon");
		Safe_Release(instance);
	}

	return instance;
}

void CMoon::Free()
{
	__super::Free();
}
