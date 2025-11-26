#include "Client_Defines.h"
#include "UI_Transition.h"
#include "GameInstance.h"
#include "IRenderService.h"
#include "IResourceService.h"
#include "Sprite2D.h"
#include "Texture.h"

CUI_Transition::CUI_Transition()
{
}

CUI_Transition::CUI_Transition(const CUI_Transition& rhs)
	:CLevelObject(rhs)
{
}

HRESULT CUI_Transition::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CSprite2D>()->Initialize(nullptr);
	auto pRcs = CGameInstance::GetInstance()->Get_ResourceMgr();
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "PostProcess.hlsl");
	Get_Component<CSprite2D>()->ChangePass("Transition_Craft");
	Get_Component<CSprite2D>()->Set_Param("MixtureTexture", { pRcs->Load_Texture(G_GlobalLevelKey,"UI_ScreenMaskUp.png")->Get_SRV(),
		"Texture2D", 0 });

	SHADER_PARAM time = { &m_fLifeTime,"float",sizeof(_float) };
	Get_Component<CSprite2D>()->Set_Param("fLifeTime", time);

	m_eRenderLayer = RENDER_LAYER::CustomOnly;
	return S_OK;
}

HRESULT CUI_Transition::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	m_pTransform->Set_Pos({ 0 - 1280 * 0.5f, -0 + 720 * 0.5f, 0.f });

	return S_OK;
}

void CUI_Transition::Priority_Update(_float dt)
{
	if (!m_bUp&&!m_bDown)
		return;
}

void CUI_Transition::Update(_float dt)
{
	if (!m_bUp && !m_bDown)
		return;

	if (m_bUp) {
		m_fLifeTime += dt * 3;

		if (m_fLifeTime > 2.f) {
			m_fLifeTime = 2.f;
			m_bUp = false;
		}
	}
	else if(m_bDown) {
		m_fLifeTime -= dt * 3;

		if (m_fLifeTime < 0.f) {
			m_fLifeTime = 0.f;
			m_bDown = false;
		}
	}
}

void CUI_Transition::Late_Update(_float dt)
{
	if (!m_bUp && !m_bDown)
		return;

	CGameInstance::GetInstance()->Get_RenderSystem()->Add_PostProcessCommand({
		"MRT_UI",
		{"MRT_UI"},
		false,
		false,
		[this](ID3D11DeviceContext* pContext) {Render_TransCraft(pContext); }
		});
}

void CUI_Transition::Render_TransCraft(ID3D11DeviceContext* pContext)
{
	auto RenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	auto sprite = Get_Component<CSprite2D>();

	if (Get_Component<CSprite2D>()->Get_CompActive() == false) return;

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

void CUI_Transition::Set_Active()
{
	m_bUp = true;
	m_fLifeTime = 0.f;
}

void CUI_Transition::Set_DeActive()
{
	m_bDown = true;
	m_fLifeTime = 2.f;
}

void CUI_Transition::Render_GUI()
{
}

CUI_Transition* CUI_Transition::Create()
{
	CUI_Transition* instance = new CUI_Transition();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CUI_Transition");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CUI_Transition::Clone(INIT_DESC* pArg)
{
	CUI_Transition* instance = new CUI_Transition(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_Responcer");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_Transition::Free()
{
	__super::Free();
}
