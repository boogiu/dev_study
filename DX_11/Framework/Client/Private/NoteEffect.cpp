#include "Client_Defines.h"
#include "NoteEffect.h"
#include "RectModel.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "Helper_Func.h"

CNoteEffect::CNoteEffect()
{
}

CNoteEffect::CNoteEffect(const CNoteEffect& rhs)
	: CBaseEffect(rhs)
{
}

HRESULT CNoteEffect::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CRectModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CNoteEffect::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();
	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	IRenderService* pRenderSys = CGameInstance::GetInstance()->Get_RenderSystem();

	pRcsMgr->Add_ResourcePath("Effect_noiz01.png", "../../Resources/Effect/noiz01.png");

	pRcsMgr->Add_ResourcePath("Effect_note01.png", "../../Resources/Effect/note01.png");
	pRcsMgr->Add_ResourcePath("Effect_note02.png", "../../Resources/Effect/note02.png");
	pRcsMgr->Add_ResourcePath("Effect_note03.png", "../../Resources/Effect/note03.png");
	pRcsMgr->Add_ResourcePath("Effect_note04.png", "../../Resources/Effect/note04.png");

	CMaterial* pMaterial = Get_Component<CMaterial>();
	CMaterialInstance* customInstance = CMaterialInstance::Create_Handle("Rect_Effect_Base", "Note", pDevice);
	m_pMaterial_Instance = customInstance;

	m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_note01.png", TEXTURE_TYPE::ALBEDO);
	m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_note02.png", TEXTURE_TYPE::ALBEDO);
	m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_note03.png", TEXTURE_TYPE::ALBEDO);
	m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_note04.png", TEXTURE_TYPE::ALBEDO);

	//m_pMaterial_Instance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "Effect_noiz01.png", TEXTURE_TYPE::MIX);

	pMaterial->Insert_MaterialInstance(m_pMaterial_Instance, nullptr);
	auto MaterialDat = m_pMaterial_Instance->Get_MaterialData();

	if (MaterialDat)
		MaterialDat->Link_Shader(G_GlobalLevelKey, "VTX_NorTexEffect.hlsl");

	m_pMaterial_Instance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
	m_pMaterial_Instance->Set_Param("fRadian", { &m_fRadian,"float",sizeof(float) });

	return S_OK;
}

void CNoteEffect::Priority_Update(_float dt)
{
	m_fLifeTime += dt;
}

void CNoteEffect::Update(_float dt)
{
	float damping = powf(0.98f, dt * 60.f);

	m_vDirection.x *=damping;
	m_vDirection.y *=damping;

	m_pTransform->Translate(XMLoadFloat3(&m_vDirection)*dt);
}

void CNoteEffect::Late_Update(_float dt)
{
}

void CNoteEffect::Render_GUI()
{
}

_bool CNoteEffect::isEffectActive()
{
	return m_fLifeTime < 2.5f;
}

void CNoteEffect::Reset()
{
	m_fLifeTime = 0.f;
}

void CNoteEffect::Set_DeActive()
{
	m_isAlive = false;
}

void CNoteEffect::Set_ReActive(const EffectData& data)
{
	Reset();
	_int sprite = Helper::Get_Random_Int(0, 3);
	_float x = Helper::Get_Random_Float(-8, 8);
	_float z = Helper::Get_Random_Float(-1, 1);
	_float y = Helper::Get_Random_Float(6, 15);

	m_vDirection = {x,y,z};
	m_fRadian = atan2(x, y);
	m_isAlive = true;
	m_pTransform->Set_Pos({ data.FxPosition.x ,data.FxPosition.y +8 ,data.FxPosition.z});

	_float sx = Helper::Get_Random_Float(1, 3);
	_float sy = Helper::Get_Random_Float(1, 3);
	m_pTransform->Scale({ sx,sy,1.f});

	m_pMaterial_Instance->ChangeTexture(TEXTURE_TYPE::ALBEDO, sprite);
}

CNoteEffect* CNoteEffect::Create()
{
	CNoteEffect* instance = new CNoteEffect();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CNoteEffect");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CNoteEffect::Clone(INIT_DESC* pArg)
{
	CNoteEffect* instance = new CNoteEffect(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CNoteEffect");
		Safe_Release(instance);
	}

	return instance;
}

void CNoteEffect::Free()
{
	__super::Free();
}
