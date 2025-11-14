#include "Client_Defines.h"
#include "Target_Texture.h"
#include "Sprite2D.h"
#include "GameInstance.h"
#include "IRenderService.h"
#include "IResourceService.h"

CTarget_Texture::CTarget_Texture()
{
}

CTarget_Texture::CTarget_Texture(const CTarget_Texture& rhs)
	:CUI_Object(rhs)
{
}

CTarget_Texture::~CTarget_Texture()
{
}

HRESULT CTarget_Texture::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	m_eRenderLayer = RENDER_LAYER::CustomOnly;
	m_bAttachParent = false;
	return S_OK;
}

HRESULT CTarget_Texture::Initialize(INIT_DESC* pArg)
{
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "UI_PartsShader.hlsl");
	__super::Initialize(pArg);
	return S_OK;
}

void CTarget_Texture::Priority_Update(_float dt)
{
}

void CTarget_Texture::Update(_float dt)
{
}

void CTarget_Texture::Late_Update(_float dt)
{
}

void CTarget_Texture::Render_GUI()
{
}

void CTarget_Texture::UI_Active(void* pArg)
{
}

void CTarget_Texture::UI_DeActive(void* pArg)
{
}

void CTarget_Texture::TargetSize(_float2 size)
{
	m_WinSizeX = size.x;
	m_WinSizeY = size.y;

	XMStoreFloat4x4(
		&m_OrthoProject,
		XMMatrixOrthographicLH(size.x, size.y, 0.f, 1.f)
	);
}

void CTarget_Texture::Center(_float2 offset)
{
	m_fLocalX = m_WinSizeX * 0.5f+offset.x;
	m_fLocalY = m_WinSizeY * 0.5f+offset.y;
}

_float2 CTarget_Texture::Target_Center(_float2 offset)
{
	return _float2(m_fLocalX, m_fLocalY);
}

_float CTarget_Texture::Target_L(_float offset)
{
	return m_fLocalX- m_fLocalX *0.5f+offset;
}

_float CTarget_Texture::Target_R(_float offset)
{
	return m_fLocalX - m_fLocalX * 0.5f + offset;
}

_float CTarget_Texture::Target_B(_float offset)
{
	return m_fLocalY + m_fLocalY * 0.5f + offset;
}

_float CTarget_Texture::Target_T(_float offset)
{
	return m_fLocalY - m_fLocalY * 0.5f + offset;
}

void CTarget_Texture::Render(ID3D11DeviceContext* pContext)
{
	auto RenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	auto sprite = Get_Component<CSprite2D>();

	ID3D11InputLayout* pLayout;
	RenderSys->Get_BufferInputLayout(
		sprite->Get_Buffer(),
		sprite->Get_Shader(),
		sprite->Get_PassConstant(),
		&pLayout
	);
	pContext->IASetInputLayout(pLayout);

	SHADER_PARAM worldParam{ m_pTransform->Get_WorldMatrix_Ptr(),    "float4x4", sizeof(_float4x4)};
	SHADER_PARAM orthoParam{ &m_OrthoProject, "float4x4", sizeof(_float4x4) };

	_float2 size = { m_fSizeX, m_fSizeY };
	SHADER_PARAM sizeParam{ &size, "float2", sizeof(_float2) };

	sprite->Set_Param("transformMatrix", worldParam);
	sprite->Set_Param("matOrtho", orthoParam);
	sprite->Set_Param("PartSize", sizeParam);

	sprite->Apply_Shader(pContext);
	sprite->Draw_Sprite(pContext);
}

CTarget_Texture* CTarget_Texture::Create()
{
	CTarget_Texture* instance = new CTarget_Texture();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CTarget_Texture");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CTarget_Texture::Clone(INIT_DESC* pArg)
{
	CTarget_Texture* instance = new CTarget_Texture(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CTarget_Texture");
		Safe_Release(instance);
	}

	return instance;
}

void CTarget_Texture::Free()
{
	__super::Free();
}
