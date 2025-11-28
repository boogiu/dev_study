#include "Client_Defines.h"
#include "Cloud_Space.h"
#include "RectModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"
#include "GameInstance.h"
#include "IRenderService.h"
#include "IResourceService.h"
#include "Helper_Func.h"
CCloud_Space::CCloud_Space()
{
}

CCloud_Space::CCloud_Space(const CCloud_Space& rhs)
	:CGameObject(rhs)
{
}

CCloud_Space::~CCloud_Space()
{
}

HRESULT CCloud_Space::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CRectModel>();
	Add_Component<CMaterial>();
	m_eRenderLayer = RENDER_LAYER::CustomOnly;
	return S_OK;
}

HRESULT CCloud_Space::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	auto pRcs = CGameInstance::GetInstance()->Get_ResourceMgr();
	pRcs->Add_ResourcePath("Effect_absentmindcloudblur.png", "../../Resources/Effect/absentmindcloudblur.png");
	
	CMaterial* pMaterial = Get_Component<CMaterial>();
	customInstance = CMaterialInstance::Create_Handle("Rect_Cloud", "Render_Cloud", CGameInstance::GetInstance()->Get_Device());

	pMaterial->Insert_MaterialInstance(customInstance, nullptr);
	auto MaterialDat = customInstance->Get_MaterialData();

	if (MaterialDat)
		MaterialDat->Link_Shader(G_GlobalLevelKey, "PostProcess.hlsl");
	customInstance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_absentmindcloudblur.png", TEXTURE_TYPE::ALBEDO);
	

	return S_OK;
}

void CCloud_Space::Awake()
{
	m_TimeOffset = Helper::Get_Random_Float(0.f, XM_2PI);  // À§»ó ·£´ý
	m_MoveScale.x = Helper::Get_Random_Float(0.5f, 1.5f);
	m_MoveScale.y = Helper::Get_Random_Float(0.2f, 1.0f);
	m_MoveScale.z = Helper::Get_Random_Float(0.5f, 1.5f);

	m_BaseSpeed = Helper::Get_Random_Float(2.f, 4.f);
}

void CCloud_Space::Priority_Update(_float dt)
{

}

void CCloud_Space::Update(_float dt)
{
	m_ElapsedTime += dt*0.03;
	m_ZTime += dt;
	float t = m_ElapsedTime + m_TimeOffset;
	float s = sinf(t);
	float z = sinf(m_ZTime);

	_float3 move{
		s * m_BaseSpeed * dt * m_MoveScale.x,
		s * m_BaseSpeed * dt * m_MoveScale.y,
		//s * m_BaseSpeed * dt * m_MoveScale.z
		z*dt*10
	};

	m_pTransform->Translate(XMLoadFloat3(&move));
}

void CCloud_Space::Late_Update(_float dt)
{
	
}

void CCloud_Space::Render_GUI()
{
	__super::Render_GUI();
}

void CCloud_Space::Render(ID3D11DeviceContext* pContext)
{
	auto RenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	auto rect = Get_Component<CRectModel>();
	auto mat = Get_Component<CMaterial>();

	if (Get_Component<CRectModel>()->Get_CompActive() == false) return;

	customInstance->Set_Param("transformMatrix", { m_pTransform->Get_WorldMatrix_Ptr(),"float4x4", sizeof(_float4x4) });
	customInstance->Set_Param("matViewEnvShadow", { m_matViewEnvShadow ,"float4x4", sizeof(_float4x4) });
	customInstance->Set_Param("matProjEnvShadow", { m_matProjEnvShadow,"float4x4", sizeof(_float4x4) });
	ID3D11InputLayout* pLayout;
	RenderSys->Get_InputLayout(
		rect,
		mat->Get_Shader(0),
		0,
		"Render_Cloud",
		&pLayout
	);
	pContext->IASetInputLayout(pLayout);

	mat->Apply_Material(pContext, 0);
	rect->Draw(pContext, 0);
}


CCloud_Space* CCloud_Space::Create()
{
	CCloud_Space* instance = new CCloud_Space();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CCloud_Space");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CCloud_Space::Clone(INIT_DESC* pArg)
{
	CCloud_Space* instance = new CCloud_Space(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CCloud_Space");
		Safe_Release(instance);
	}

	return instance;
}

void CCloud_Space::Free()
{
	__super::Free();
}
