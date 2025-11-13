#include "Client_Defines.h"
#include "UI_ItemCard.h"
#include "Sprite2D.h"
#include "GameInstance.h"
#include "Texture.h"
#include "IResourceService.h"
#include "ObjectContainer.h"

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

HRESULT CUI_ItemCard::Initialize(INIT_DESC * pArg)
{
	__super::Initialize(pArg);
	/*∫ª¿Œ*/
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_CardBackGruond.png");

	m_pInside = 
		Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_TexturePanel" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ m_fSizeX*0.9f,m_fSizeY * 0.9f })
		.Position({0,0 })
		.Rotate(m_fRadian)
		.Build("InsideCard");

	m_pInside->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level","UI_BackgroundOverlay.png");

	auto pMaskTexture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture("GamePlay_Level", "UI_CardPattern.png");
	auto pFrameTexture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture("GamePlay_Level", "UI_CardBackGruond.png");

	SHADER_PARAM maskParam = { pMaskTexture->Get_SRV(),"Texture2D",0 };
	SHADER_PARAM frameParam = { pFrameTexture->Get_SRV(),"Texture2D",0 };
	m_pInside->Get_Component<CSprite2D>()->ChangePass("Masking_UI");
	m_pInside->Get_Component<CSprite2D>()->Set_Param("UI_MaskTexture", maskParam);
	m_pInside->Get_Component<CSprite2D>()->Set_Param("UI_FrameTexture", frameParam);


	m_pImage =
		Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_TexturePanel" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 100,100 })
		.Position({ 0,0 })
		.Rotate(m_fRadian)
		.Build("InsideCard");

	m_pImage->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "MenuLayout_FtrWoodPile.png");


	Get_Component<CObjectContainer>()->Add_Child(m_pInside, false);
	Get_Component<CObjectContainer>()->Add_Child(m_pImage, false);

	vBaseSize.x = m_fSizeX;
	vBaseSize.y=m_fSizeY;
	return S_OK;
}

void CUI_ItemCard::Priority_Update(_float dt)
{
	m_bHover = false;
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CUI_ItemCard::Update(_float dt)
{
	if (m_bHover) {
		Size_To({ vBaseSize.x*1.1f,vBaseSize.y * 1.1f }, 8 * dt);
		m_pInside->Size_To({ m_fSizeX * 0.9f,m_fSizeY * 0.9f }, 9 * dt);
		m_pImage->Size_To({ 110,110 }, 9 * dt);
	}
	else  {
		Size_To({ vBaseSize.x,vBaseSize.y }, 8 * dt);
		m_pInside->Size_To({ m_fSizeX * 0.9f,m_fSizeY * 0.9f }, 9 * dt);
		m_pImage->Size_To({ 100,100 }, 9 * dt);
	}
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CUI_ItemCard::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CUI_ItemCard::Render_GUI()
{
}

void CUI_ItemCard::Hover()
{
	m_bHover = true;
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
