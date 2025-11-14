#include "Client_Defines.h"
#include "CraftCard.h"

#include "Sprite2D.h"
#include "GameInstance.h"
#include "Texture.h"
#include "IResourceService.h"
#include "ObjectContainer.h"
#include "IRenderService.h"

#include "UI_Text.h"
#include "Target_Texture.h"

CCraftCard::CCraftCard()
{
}

CCraftCard::CCraftCard(const CCraftCard& rhs)
	:CUI_Object(rhs)
{
}

HRESULT CCraftCard::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
	return S_OK;
}

HRESULT CCraftCard::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	/*∫ª¿Œ*/
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");

	Ready_Part();
	
	m_bActive = false;
	m_vBasePos = { m_fLocalX, m_fLocalY };
	return S_OK;
}

void CCraftCard::Awake()
{
	auto RenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	RenderTargetDesc CardDesc = {
		"CraftCard" ,
		DXGI_FORMAT_R8G8B8A8_UNORM ,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		_float4(0.f, 0.f, 0.f, 0.f) ,
		m_vTargetSize.x,m_vTargetSize.y
	};

	RenderSys->Create_RenderTarget(CardDesc);

	SHADER_PARAM textureParam{ RenderSys->Get_TargetSRV("CraftCard"), "Texture2D",0 };
	Get_Component<CSprite2D>()->Set_Param("SpriteTexture", textureParam);
}

void CCraftCard::Priority_Update(_float dt)
{
	Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	if (!m_bActive) return;
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CCraftCard::Update(_float dt)
{
	Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	if (!m_bActive) return;
	Move_To({ 0,0 }, 18 * dt);
	Rotate_To(0, 18 * dt);
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CCraftCard::Late_Update(_float dt)
{
	Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	if (!m_bActive) return;
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);

	auto RenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	RENDER_COMMAND cmd = { "CraftCard" ,
		[this](ID3D11DeviceContext* pContext)
		{Render_CraftCard(pContext); } };

	RenderSys->Add_RenderCommand(cmd);
}

void CCraftCard::Render_GUI()
{

}

void CCraftCard::Render_CraftCard(ID3D11DeviceContext* pContext)
{
	m_pBaseBackGround->Render(pContext);
	m_pImageBackGround->Render(pContext);
	m_pInfoBackGround->Render(pContext);
	m_pItemImage->Render(pContext);
	m_pNameStroke->Render(pContext);
	m_pCategoryIcon->Render(pContext);
	m_pSourceItem_01->Render(pContext);
	m_pSourceItemLine_01->Render(pContext);
	m_pSourceItemBLine_01->Render(pContext);
	m_pSourceItemCountSlot->Render(pContext);
}

void CCraftCard::UI_Active(void* pArg)
{
	m_bActive = true;
	m_fLocalX = -15;
	m_fLocalY = -15;
	m_fRadian = XMConvertToRadians(5.f);
}

void CCraftCard::UI_DeActive(void* pArg)
{
	m_bActive = false;
}

void CCraftCard::Ready_Part()
{
	m_vTargetSize = { 1280,720 };
	
	//Get_Component<CObjectContainer>()->Add_Child(psubItmeNeedCount, false);
	//psubItmeNeedCount->Align_To(ANCHOR::Left | ANCHOR::Center, m_pCountMine->Local_RB());
	//m_pCountNeed = dynamic_cast<CUI_Text*>(psubItmeNeedCount);
	//m_pCountNeed->Set_Text(L"/ 5");
	//m_pCountNeed->Set_Active(true);
	//m_pCountNeed->Set_Anchor(ANCHOR::Left | ANCHOR::Center);
	//m_pCountNeed->Set_Scale(0.7f);
	//m_pCountNeed->Set_Color(Brown);

	auto RcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	CUI_Object* pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
	.Add_To_Level("GamePlay_Level")
	.Build("InsideCard");
	m_pBaseBackGround = dynamic_cast<CTarget_Texture*>(pImage);
	m_pBaseBackGround->TargetSize(m_vTargetSize);
	m_pBaseBackGround->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_BackgroundOverlay.png");
	m_pBaseBackGround->Set_Size(m_vTargetSize);
	m_pBaseBackGround->Center();
	Get_Component<CObjectContainer>()->Add_Child(m_pBaseBackGround, false);

	pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build("InsideCard");
	m_pImageBackGround = dynamic_cast<CTarget_Texture*>(pImage);
	m_pImageBackGround->TargetSize(m_vTargetSize);
	m_pImageBackGround->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_RecipeInfoBackground.png");
	m_pImageBackGround->Set_Size(_vector{550,500});
	m_pImageBackGround->Center({ 280, -25.f});
	Get_Component<CObjectContainer>()->Add_Child(m_pImageBackGround, false);

	pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build("InsideCard");
	m_pInfoBackGround = dynamic_cast<CTarget_Texture*>(pImage);
	m_pInfoBackGround->TargetSize(m_vTargetSize);
	m_pInfoBackGround->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_CraftIconImageMask.png");
	auto pMaskTexture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture("GamePlay_Level", "UI_CraftIconMask.png");
	SHADER_PARAM maskParam = { pMaskTexture->Get_SRV(),"Texture2D",0 };
	m_pInfoBackGround->Get_Component<CSprite2D>()->Set_Param("UI_MaskTexture", maskParam);
	m_pInfoBackGround->Get_Component<CSprite2D>()->ChangePass("Inside_Empty");
	m_pInfoBackGround->Set_Size(_vector{ 500, 450 });
	m_pInfoBackGround->Center({ -280, 25.f });
	Get_Component<CObjectContainer>()->Add_Child(m_pInfoBackGround, false);


	pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build("InsideCard");
	m_pItemImage = dynamic_cast<CTarget_Texture*>(pImage);
	m_pItemImage->TargetSize(m_vTargetSize);
	m_pItemImage->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "MenuLayout_FtrWoodPile.png");
	m_pItemImage->Set_Size(_vector{ 300, 300 });
	m_pItemImage->Center({ -280, 25.f });
	Get_Component<CObjectContainer>()->Add_Child(m_pItemImage, false);

	pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build("InsideCard");
	m_pNameStroke = dynamic_cast<CTarget_Texture*>(pImage);
	m_pNameStroke->TargetSize(m_vTargetSize);
	m_pNameStroke->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_LineStroke.png");
	m_pNameStroke->Set_Size(_vector{ 500, 3.f });
	m_pNameStroke->Center({ -280, -250});
	Get_Component<CObjectContainer>()->Add_Child(m_pNameStroke, false);
	
	pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build("InsideCard");
	m_pCategoryIcon = dynamic_cast<CTarget_Texture*>(pImage);
	m_pCategoryIcon->TargetSize(m_vTargetSize);
	m_pCategoryIcon->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_IconCatFurniture.png");
	m_pCategoryIcon->Set_Size(_vector{ 50,50 });
	m_pCategoryIcon->Center({ -510, -285 });
	Get_Component<CObjectContainer>()->Add_Child(m_pCategoryIcon, false);


	pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build("InsideCard");
	m_pCategoryIcon = dynamic_cast<CTarget_Texture*>(pImage);
	m_pCategoryIcon->TargetSize(m_vTargetSize);
	m_pCategoryIcon->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_IconCatFurniture.png");
	m_pCategoryIcon->Set_Size(_vector{ 50,50 });
	m_pCategoryIcon->Center({ -510, -285 });
	Get_Component<CObjectContainer>()->Add_Child(m_pCategoryIcon, false);


	pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build("InsideCard");
	m_pSourceItem_01 = dynamic_cast<CTarget_Texture*>(pImage);
	m_pSourceItem_01->TargetSize(m_vTargetSize);
	m_pSourceItem_01->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "MenuLayout_DIYWoodNormal.png");
	m_pSourceItem_01->Set_Size(_vector{ 55,55 });
	m_pSourceItem_01->Center({ 55, -225.f });
	Get_Component<CObjectContainer>()->Add_Child(m_pSourceItem_01, false);

	pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build("InsideCard");
	m_pSourceItemLine_01 = dynamic_cast<CTarget_Texture*>(pImage);
	m_pSourceItemLine_01->TargetSize(m_vTargetSize);
	m_pSourceItemLine_01->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_LindeStroke2.png");
	m_pSourceItemLine_01->Set_Size(_vector{ 200,3 });
	m_pSourceItemLine_01->Center({ 300, -225.f });
	Get_Component<CObjectContainer>()->Add_Child(m_pSourceItemLine_01, false);

	pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build("InsideCard");
	m_pSourceItemBLine_01 = dynamic_cast<CTarget_Texture*>(pImage);
	m_pSourceItemBLine_01->TargetSize(m_vTargetSize);
	m_pSourceItemBLine_01->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_TextLine.png");
	m_pSourceItemBLine_01->Set_Size(_vector{ 400,3 });
	m_pSourceItemBLine_01->Center({ 300, -200.f });
	Get_Component<CObjectContainer>()->Add_Child(m_pSourceItemBLine_01, false);

	pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build("InsideCard");
	m_pSourceItemBLine_01 = dynamic_cast<CTarget_Texture*>(pImage);
	m_pSourceItemBLine_01->TargetSize(m_vTargetSize);
	m_pSourceItemBLine_01->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_TextLine.png");
	m_pSourceItemBLine_01->Set_Size(_vector{ 400,3 });
	m_pSourceItemBLine_01->Center({ 300, -200.f });
	Get_Component<CObjectContainer>()->Add_Child(m_pSourceItemBLine_01, false);

	pImage = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build("InsideCard");
	m_pSourceItemCountSlot = dynamic_cast<CTarget_Texture*>(pImage);
	m_pSourceItemCountSlot->TargetSize(m_vTargetSize);
	m_pSourceItemCountSlot->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_SlotRect.png");
	m_pSourceItemCountSlot->Set_Size(_vector{ 40,40 });
	m_pSourceItemCountSlot->Center({ 425, -225.f });
	Get_Component<CObjectContainer>()->Add_Child(m_pSourceItemCountSlot, false);
}

CCraftCard* CCraftCard::Create()
{
	CCraftCard* instance = new CCraftCard();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CCraftCard");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CCraftCard::Clone(INIT_DESC* pArg)
{
	CCraftCard* instance = new CCraftCard(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CCraftCard");
		Safe_Release(instance);
	}

	return instance;
}

void CCraftCard::Free()
{
	__super::Free();
}