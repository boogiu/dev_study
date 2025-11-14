#include "Client_Defines.h"
#include "UI_ItemCard.h"
#include "Sprite2D.h"
#include "GameInstance.h"
#include "Texture.h"
#include "IResourceService.h"
#include "ObjectContainer.h"
#include "IRenderService.h"
#include "Target_Texture.h"
#include "IUI_Service.h"

CUI_ItemCard::CUI_ItemCard()
{
}

CUI_ItemCard::CUI_ItemCard(const CUI_ItemCard& rhs)
	:CUI_Object(rhs)
{
}

HRESULT CUI_ItemCard::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
	return S_OK;
}

HRESULT CUI_ItemCard::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	/*∫ª¿Œ*/
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Set_CompActive(true);

	vBaseSize.x = m_fSizeX;
	vBaseSize.y = m_fSizeY;
	fBaseRadian = m_fRadian;
	m_InstanceName = "Card" + to_string(m_ObjectID);
	Ready_Part();

	return S_OK;
}

void CUI_ItemCard::Awake()
{
	auto RenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	RenderTargetDesc CardDesc = { 
		"CardTexture" ,
		DXGI_FORMAT_R8G8B8A8_UNORM ,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		_float4(0.f, 0.f, 0.f, 0.f) ,
		m_fSizeX*5,
		m_fSizeY * 5
	};

	RenderSys->Create_RenderTarget(CardDesc);

	SHADER_PARAM textureParam{ RenderSys->Get_TargetSRV("CardTexture"), "Texture2D",0 };
	Get_Component<CSprite2D>()->Set_Param("SpriteTexture", textureParam);
}

void CUI_ItemCard::Priority_Update(_float dt)
{
	m_bHover = false;
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CUI_ItemCard::Update(_float dt)
{
	if (m_bHover) {
		Size_To({ vBaseSize.x * 1.2f,vBaseSize.y * 1.2f }, 8.f * dt);
		Rotate_To(fBaseRadian+XMConvertToRadians(10), 8.f * dt);
	}
	else {
		Size_To({ vBaseSize.x,vBaseSize.y }, 8.f * dt);
		Rotate_To(fBaseRadian, 8.f * dt);
	}

	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CUI_ItemCard::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);

	auto RenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	RENDER_COMMAND cmd = { "CardTexture" ,
		[this](ID3D11DeviceContext* pContext) 
		{Render_Card(pContext); } };

	RenderSys->Add_RenderCommand(cmd);
}

void CUI_ItemCard::Render_GUI()
{
}

void CUI_ItemCard::UI_Active(void* pArg)
{
}

void CUI_ItemCard::UI_DeActive(void* pArg)
{
}
void CUI_ItemCard::Render_Card(ID3D11DeviceContext* pContext)
{
	m_pBaseBackGround->Render(pContext);
	m_pMaskedBackGround->Render(pContext);
	m_pIconImage->Render(pContext);
}

void CUI_ItemCard::Hover()
{
	m_bHover = true;
}

void CUI_ItemCard::Ready_Part()
{
	auto RcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	CUI_Object* pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build("InsideCard");
	m_pBaseBackGround = dynamic_cast<CTarget_Texture*>(pImage);

	 pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build("InsideCard");
	m_pMaskedBackGround = dynamic_cast<CTarget_Texture*>(pImage);

	pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build("InsideCard");
	m_pIconImage = dynamic_cast<CTarget_Texture*>(pImage);

	m_pMaskedBackGround->TargetSize({ m_fSizeX * 5,m_fSizeY * 5 });
	m_pIconImage->TargetSize({ m_fSizeX * 5,m_fSizeY * 5 });
	m_pBaseBackGround->TargetSize({ m_fSizeX * 5,m_fSizeY * 5 });

	m_pBaseBackGround->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level","UI_CardBackGruond.png");
	m_pMaskedBackGround->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level","UI_CardInnerBack.png");
	SHADER_PARAM maskParam{ RcsMgr->Load_Texture("GamePlay_Level","UI_CardPattern.png")->Get_SRV(), "Texture2D",0};
	m_pMaskedBackGround->Get_Component<CSprite2D>()->Set_Param("UI_MaskTexture", maskParam);
	m_pMaskedBackGround->Get_Component<CSprite2D>()->ChangePass("Masking_UI");
	m_pIconImage->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level","MenuLayout_FtrWoodPile.png");

	Get_Component<CObjectContainer>()->Add_Child(m_pBaseBackGround, false);
	Get_Component<CObjectContainer>()->Add_Child(m_pMaskedBackGround, false);
	Get_Component<CObjectContainer>()->Add_Child(m_pIconImage, false);

	m_pBaseBackGround->Set_Size(_vector{ m_fSizeX * 5,m_fSizeY * 5 });
	m_pBaseBackGround->Center();
	m_pMaskedBackGround->Set_Size(_vector{ m_fSizeX * 4.6f,m_fSizeY * 4.6f });
	m_pMaskedBackGround->Center();
	m_pIconImage->Set_Size(_vector{ m_fSizeX*3 ,m_fSizeX * 3 });
	m_pIconImage->Center();

}

CUI_ItemCard* CUI_ItemCard::Create()
{
	CUI_ItemCard* instance = new CUI_ItemCard();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CUI_ItemCard");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CUI_ItemCard::Clone(INIT_DESC* pArg)
{
	CUI_ItemCard* instance = new CUI_ItemCard(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_ItemCard");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_ItemCard::Free()
{
	__super::Free();
}
