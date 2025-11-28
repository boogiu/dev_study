#include "Client_Defines.h"
#include "Cloud_Env.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "RectModel.h"
#include "UI_Object.h"
#include "ObjectContainer.h"
#include "Camera.h"
#include "Sprite2D.h"
#include "Cloud_Space.h"
#include "PipeLine.h"
#include "Helper_Func.h"
CCloud_Env::CCloud_Env()
{
}

CCloud_Env::CCloud_Env(const CCloud_Env& rhs)
	:CGameObject(rhs)
{
}

CCloud_Env::~CCloud_Env()
{
}

HRESULT CCloud_Env::Initialize_Prototype()
{

	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
	Add_Component<CSprite2D>();
	Add_Component<CCamera>();
	return S_OK;
}

HRESULT CCloud_Env::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg); //GamePlay_Env_Cloud_Space
	Get_Component<CCamera>()->Set_Far(1000);
	for (size_t i = 0; i < 15; i++)
	{
		_float x = Helper::Get_Random_Float(-550, 550);
		_float y = Helper::Get_Random_Float(-250, 250);
		_float z = Helper::Get_Random_Float(450, 850);

		_float scalex = Helper::Get_Random_Float(50, 150);
		_float scaley = Helper::Get_Random_Float(50, 150);

		CGameObject* pObj = Builder::Create_Object({ "GamePlay_Level", "GamePlay_Env_Cloud_Space" })
			.Position({ x,y,z })
			.Scale({ scalex,scaley,5 })
			.Build("CloudSpace");

		CCloud_Space* cloud = dynamic_cast<CCloud_Space*>(pObj);
		Get_Component<CObjectContainer>()->Add_Child(pObj, TRUE);
		cloud->Set_Matrix(&m_ViewMatrix, &m_ProjMatrix);
		m_Clouds.push_back(cloud);
	}

	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "PostProcess.hlsl");
	Get_Component<CSprite2D>()->ChangePass("Env_Cloud");

	return S_OK;
}

void CCloud_Env::Awake()
{
	auto RenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	_float2 WinSize = CGameInstance::GetInstance()->Get_ClientSize();


	RenderTargetDesc CloudTexture = {
		"CloudTexture" ,
		DXGI_FORMAT_R8G8B8A8_UNORM ,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		_float4(0,0,0,0) ,
		g_iMaxWidth,g_iMaxHeight
	};

	RenderSys->Create_RenderTarget(CloudTexture);

	SHADER_PARAM viewParam{ &m_ViewMatrix, "float4x4",sizeof(_float4x4) };
	SHADER_PARAM projParam{ &m_ProjMatrix, "float4x4",sizeof(_float4x4) };
	SHADER_PARAM textureParam{ RenderSys->Get_CustomTargetSRV("CloudTexture"), "Texture2D",0 };
	SHADER_PARAM depthParam{ RenderSys->Get_EngineTargetSRV("Target_Depth"), "Texture2D",0 }; \

		Get_Component<CSprite2D>()->Set_Param("g_CloudTexture", textureParam);
	Get_Component<CSprite2D>()->Set_Param("g_DepthTexture", depthParam);
	Get_Component<CSprite2D>()->Set_Param("matViewEnvShadow", viewParam);
	Get_Component<CSprite2D>()->Set_Param("matProjEnvShadow", projParam);
}

void CCloud_Env::Priority_Update(_float dt)
{

	m_pTransform->LookAt(m_pTransform->Get_Pos() + _vector{ 0,-1, 0 });

	m_ViewMatrix = Get_Component<CCamera>()->Get_ViewMatrix();
	XMStoreFloat4x4(&m_ProjMatrix, Get_Component<CCamera>()->Get_ProjMatrix());

	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CCloud_Env::Update(_float dt)
{
	Get_Component<CObjectContainer>()->UpdateChild(dt);

}

void CCloud_Env::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);

	auto RenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	RENDER_CUSTOM_COMMAND cmd = { "CloudTexture" ,[this](ID3D11DeviceContext* pContext) {Render_CloudChild(pContext); } };
	RenderSys->Add_RenderCommand(cmd);

	CGameInstance::GetInstance()->Get_RenderSystem()->Add_PostProcessCommand({
		"MRT_Final",
		{"MRT_Final"},
		false,
		false,
		[this](ID3D11DeviceContext* pContext) {Render_Cloud(pContext); }
		});
}

void CCloud_Env::Render_Cloud(ID3D11DeviceContext* pContext)
{
	auto RenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	auto sprite = Get_Component<CSprite2D>();

	if (Get_Component<CSprite2D>()->Get_CompActive() == false) return;
	auto pPipeLine = RenderSys->Get_Pipeline();
	sprite->Get_Shader()->SetConstantBuffer("FrameBuffer", pPipeLine->Get_FrameBuffer());

	ID3D11InputLayout* pLayout;

	RenderSys->Get_BufferInputLayout(
		sprite->Get_Buffer(),
		sprite->Get_Shader(),
		sprite->Get_PassConstant(),
		&pLayout
	);

	pContext->IASetInputLayout(pLayout);
	sprite->Apply_Shader(pContext);
	sprite->Draw_Sprite(pContext);
}

void CCloud_Env::Render_CloudChild(ID3D11DeviceContext* pContext)
{
	for (auto& Cloud : m_Clouds)
	{
		Cloud->Render(pContext);
	}
}

void CCloud_Env::Render_GUI()
{
	__super::Render_GUI();
}

CCloud_Env* CCloud_Env::Create()
{
	CCloud_Env* instance = new CCloud_Env();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CCloud_Env");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CCloud_Env::Clone(INIT_DESC* pArg)
{
	CCloud_Env* instance = new CCloud_Env(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CCloud_Env");
		Safe_Release(instance);
	}

	return instance;
}
void CCloud_Env::Free()
{
	__super::Free();
}
