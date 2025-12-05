#include "Client_Defines.h"
#include "CraftCard.h"

#include "Sprite2D.h"
#include "GameInstance.h"
#include "Texture.h"
#include "IResourceService.h"
#include "ObjectContainer.h"
#include "IRenderService.h"
#include "UI_Cursor.h"

#include "UI_Text.h"
#include "Target_Texture.h"
#include "Target_Text.h"
#include "UI_CraftPanel.h"
#include "AudioSource.h"

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

	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "UI_Decide.wav", "Select");
	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "UI_Invalid.wav", "Invalid");
	Get_Component<CAudioSource>()->Set_3DAttribute("Select", false);
	Get_Component<CAudioSource>()->Set_SlotVolume("Select", 0.3f);

	Get_Component<CAudioSource>()->Set_3DAttribute("Invalid", false);
	Get_Component<CAudioSource>()->Set_SlotVolume("Invalid", 0.3f);
	return S_OK;
}

HRESULT CCraftCard::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	/*본인*/
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

	SHADER_PARAM textureParam{ RenderSys->Get_CustomTargetSRV("CraftCard"), "Texture2D",0 };
	Get_Component<CSprite2D>()->Set_Param("SpriteTexture", textureParam);
}

void CCraftCard::Priority_Update(_float dt)
{
	Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	if (!m_bActive) return;
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);

	if (m_fElpasedTime >0.5f&&CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_SPACE)) {
		if(m_data.isAbleToCraft())
		{
			ButtonOn = true;
			Get_Component<CAudioSource>()->Play("Select");
		}
		else
			Get_Component<CAudioSource>()->Play("Invalid");
	}
}

void CCraftCard::Update(_float dt)
{
	Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	if (!m_bActive) return;

	m_fElpasedTime += dt;
	Rotate_To(0, 18 * dt);
	_bool OpenComplte = Move_To({ 0,0 }, 18 * dt);

	m_pCursor->Set_Pivot({ 280,300 }, { 100,10}, { 1.f, 1.f });

	if (ButtonOn) {
		m_Templete.pButtonBG->Size_To({ 210, 60 }, 8*dt);
		m_Templete.pButton->Size_To({195,50}, 8 * dt);
		m_Templete.pButtonEffect->Get_Component<CSprite2D>()->Set_CompActive(true);
		m_fEffectTime += dt*10;

		m_Templete.pButtonEffect->Get_Component<CSprite2D>()->ChangeSprite(static_cast<_uint>(m_fEffectTime));
		
		if (m_fEffectTime > 1.5f) {
			m_Templete.pButtonBG->Size_To({ 230, 80 }, 8 * dt);
			m_Templete.pButton->Size_To({ 215,70 }, 8 * dt);
		}

		if (m_fEffectTime > 3.f) {
			m_Templete.pButtonEffect->Get_Component<CSprite2D>()->Set_CompActive(false);
			m_pPanel->Craft_Selected(m_data);
		}
	}


	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CCraftCard::Late_Update(_float dt)
{
	Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	if (!m_bActive) return;
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);

	auto RenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	RENDER_CUSTOM_COMMAND cmd = { "CraftCard" ,
		[this](ID3D11DeviceContext* pContext)
		{Render_CraftCard(pContext); } };

	RenderSys->Add_RenderCommand(cmd);
}

void CCraftCard::Render_GUI()
{

}

void CCraftCard::Render_CraftCard(ID3D11DeviceContext* pContext)
{
	m_Templete.Render(pContext);
	m_pItemImage->Render(pContext);
	m_pCategoryIcon->Render(pContext);
	m_Slot1.Render(pContext);
	m_Slot2.Render(pContext);
	m_Slot3.Render(pContext);
}

void CCraftCard::UI_Active(void* pArg)
{
	m_bActive = true;
	m_fLocalX = -15;
	m_fLocalY = -15;
	m_fRadian = XMConvertToRadians(5.f);
	ButtonOn = false;
	m_fEffectTime = 0;
	m_fElpasedTime = 0.f;  
	m_Templete.pButtonEffect->Get_Component<CSprite2D>()->Set_CompActive(false);
}

void CCraftCard::UI_DeActive(void* pArg)
{
	m_Templete.pButtonEffect->Get_Component<CSprite2D>()->Set_CompActive(false);
	m_bActive = false;
}

void CCraftCard::Set_Data(CraftData data)
{
	m_data = data; 
	m_pItemImage->Get_Component<CSprite2D>()->Change_Texture(0,"GamePlay_Level", data.RecipeImage);
	m_pRecipeName->Get_Component<CTextSlot>()->Set_Text(data.Result_Item.ItemName);

	if (data.ResourceItemCount_01 != 0) {
		m_Slot1.Active();
		m_Slot1.pSourceItem->Get_Component<CSprite2D>()->Change_Texture(0, "GamePlay_Level", data.ResourceItem_01.IconName);
		m_Slot1.pRecipeItemName->Get_Component<CTextSlot>()->Set_Text(data.ResourceItem_01.ItemName);
		m_Slot1.pRecipeItemCount->Get_Component<CTextSlot>()->Set_Text(to_wstring(data.nowCount_01));
		m_Slot1.pRecipeItemNeed->Get_Component<CTextSlot>()->Set_Text(L" / " + to_wstring(data.ResourceItemCount_01));
		if (data.ResourceItemCount_01 > data.nowCount_01) {
			m_Slot1.pRecipeItemCount->Set_Color(Red);
		}
		else {
			m_Slot1.pRecipeItemCount->Set_Color(Brown);
		}
	}
	else {
		m_Slot1.DeActive();
	}

	if (data.ResourceItemCount_02 != 0) {
		m_Slot2.Active();
		m_Slot2.pSourceItem->Get_Component<CSprite2D>()->Change_Texture(0, "GamePlay_Level", data.ResourceItem_02.IconName);
		m_Slot2.pRecipeItemName->Get_Component<CTextSlot>()->Set_Text(data.ResourceItem_02.ItemName);
		m_Slot2.pRecipeItemCount->Get_Component<CTextSlot>()->Set_Text(to_wstring(data.nowCount_02));
		m_Slot2.pRecipeItemNeed->Get_Component<CTextSlot>()->Set_Text(L" / " + to_wstring(data.ResourceItemCount_02));
		if (data.ResourceItemCount_02 > data.nowCount_02) {
			m_Slot2.pRecipeItemCount->Set_Color(Red);
		}
		else {
			m_Slot2.pRecipeItemCount->Set_Color(Brown);
		}
	}
	else {
		m_Slot2.DeActive();
	}

	if (data.ResourceItemCount_03 != 0) {
		m_Slot3.Active();
		m_Slot3.pSourceItem->Get_Component<CSprite2D>()->Change_Texture(0, "GamePlay_Level", data.ResourceItem_03.IconName);
		m_Slot3.pRecipeItemName->Get_Component<CTextSlot>()->Set_Text(data.ResourceItem_03.ItemName);
		m_Slot3.pRecipeItemCount->Get_Component<CTextSlot>()->Set_Text(to_wstring(data.nowCount_03));
		m_Slot3.pRecipeItemNeed->Get_Component<CTextSlot>()->Set_Text(L" / " + to_wstring(data.ResourceItemCount_03));
		if (data.ResourceItemCount_03 > data.nowCount_03) {
			m_Slot3.pRecipeItemCount->Set_Color(Red);
		}
		else {
			m_Slot3.pRecipeItemCount->Set_Color(Brown);
		}
	}
	else {
		m_Slot3.DeActive();
	}
}

void CCraftCard::Ready_Part()
{
	m_vTargetSize = { 1280,720 };
	
	auto RcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	m_Templete.pBaseBackGround = MakePart("UI_BackgroundOverlay.png", m_vTargetSize, { 0,0 }, "", "BaseBG");
	m_Templete.pInfoBackGround = MakePart("UI_RecipeInfoBackground.png", { 550,500 }, { 280,-25 }, "", "ImageBG");
	m_Templete.pNameStroke = MakePart("UI_LineStroke.png", { 500,3 }, { -280,-250 }, "", "NameStroke");
	m_Templete.pRecipeDial = MakeText(L"재료(주머니에 있는 개수 / 필요한 개수)", { -0, -20 }, .8f, { 15,-285 }, Brown, "NameText");

	auto pImageBG = MakePart("UI_CraftIconImageMask.png", { 500, 450 }, { -280, 25.f }, "Inside_Empty", "InfoBG");
	auto pMaskTexture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture("GamePlay_Level", "UI_CraftIconMask.png");
	SHADER_PARAM maskParam = { pMaskTexture->Get_SRV(),"Texture2D",0 };
	pImageBG->Get_Component<CSprite2D>()->Set_Param("UI_MaskTexture", maskParam);
	m_Templete.pImageBackGround = pImageBG;

	m_pItemImage = MakePart("MenuLayout_FtrWoodPile.png", { 300,300 }, { -280,25 }, "", "ItemImg");
	m_pCategoryIcon = MakePart("UI_IconCatFurniture.png", { 50,50 }, { -510, -285 }, "", "CategoryIcon");
	m_pRecipeName = MakeText(L"나무 말뚝", { 0, 20 }, 1.4f, { -460,-265 }, Brown, "NameText");

	/*00, x중간, 중간*/
	m_Slot1.pSourceItem = MakePart("MenuLayout_DIYWoodNormal.png",		{ 55,55 }, { 55, -225.f }, "", "SourceItem1");
	m_Slot1.pSourceItemLine = MakePart("UI_LindeStroke2.png", { 200,3 },	{ 300, -225.f }, "", "SourceItemLine1");
	m_Slot1.pSourceItemBLine = MakePart("UI_TextLine.png", { 400,3 },			{ 300, -200.f }, "", "SourceItemBLine_01");
	m_Slot1.pSourceItemCountSlot = MakePart("UI_SlotRect.png", { 40,40 }, { 425, -225.f }, "", "SlotRect");

	m_Slot1.pRecipeItemName = MakeText(L"목재", { -0, -20 }, 1.f, { 80,	-225.f }, Brown, "NameText");
	m_Slot1.pRecipeItemCount = MakeText(L"3", { -0, -20 }, 1.f, { 390,	-220.f }, Brown, "NameText");
	m_Slot1.pRecipeItemNeed = MakeText(L"/  5", { -0, -20 }, .7f, { 425, -220.f }, Brown, "NameText");


	m_Slot2.pSourceItem = MakePart("MenuLayout_DIYWoodNormal.png", { 55,55 }, { 55, -150.f }, "", "SourceItem2");
	m_Slot2.pSourceItemLine = MakePart("UI_LindeStroke2.png", { 200,3 }, { 300, -150.f }, "", "SourceItemLine2");
	m_Slot2.pSourceItemBLine = MakePart("UI_TextLine.png", { 400,3 }, { 300, -125.f }, "", "SourceItemBLine_02");
	m_Slot2.pSourceItemCountSlot = MakePart("UI_SlotRect.png", { 40,40 }, { 425, -150.f }, "", "SlotRect2");
	m_Slot2.pRecipeItemName = MakeText(L"목재", { -0, -20 }, 1.f, { 80,	-150.f }, Brown, "NameText");
	m_Slot2.pRecipeItemCount = MakeText(L"3", { -0, -20 }, 1.f, { 390,	-147.f }, Brown, "NameText");
	m_Slot2.pRecipeItemNeed = MakeText(L"/  5", { -0, -20 }, .7f, { 425, -145.f }, Brown, "NameText");


	m_Slot3.pSourceItem = MakePart("MenuLayout_DIYWoodNormal.png", { 55,55 }, { 55, -75.f }, "", "SourceItem3");
	m_Slot3.pSourceItemLine = MakePart("UI_LindeStroke2.png", { 200,3 }, { 300, -75.f }, "", "SourceItemLine3");
	m_Slot3.pSourceItemBLine = MakePart("UI_TextLine.png", { 400,3 }, { 300, -50.f }, "", "SourceItemBLine_03");
	m_Slot3.pSourceItemCountSlot = MakePart("UI_SlotRect.png", { 40,40 }, { 425, -75.f }, "", "SlotRect3");
	m_Slot3.pRecipeItemName = MakeText(L"목재", { -0, -20 }, 1.f, { 80,	-75.f }, Brown, "NameText");
	m_Slot3.pRecipeItemCount = MakeText(L"3", { -0, -20 }, 1.f, { 390,	-75.f }, Brown, "NameText");
	m_Slot3.pRecipeItemNeed = MakeText(L"/  5", { -0, -20 }, .7f, { 425, -70.f }, Brown, "NameText");

	/*커서*/
	CUI_Object* pCursor = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Cursor" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 80,80 })
		.Position({ 0, 0 })
		.Build("Dialcursor");

	m_pCursor = dynamic_cast<CUI_Cursor*>(pCursor);
	Get_Component<CObjectContainer>()->Add_Child(m_pCursor, false);
	m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(true);

	m_Templete.pButtonBG = MakePart("UI_Button_Back.png", { 230,80 }, { 280, 300.f }, "", "ButtonBG");
	m_Templete.pButton = MakePart("UI_Button_Back.png", { 215,70 }, { 280, 300.f }, "Flow_Pattern", "ButtonBG");
	auto pBtnMaskTexture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture("GamePlay_Level", "UI_DeviceAppPattern.png");
	SHADER_PARAM BtnmaskParam = { pBtnMaskTexture->Get_SRV(),"Texture2D",0 };
	SHADER_PARAM timeParam = { &m_fElpasedTime,"float",sizeof(float)};
	m_Templete.pButton ->Get_Component<CSprite2D>()->Set_Param("UI_MaskTexture", BtnmaskParam);
	m_Templete.pButton ->Get_Component<CSprite2D>()->Set_Param("fLifeTime", timeParam);
	MakeText(L"이걸 만들래!", { -0, -20 }, 1.f, { 180, 280.f }, Ibory, "NameText");

	m_Templete.pButtonEffect = MakePart("UI_MenuEffect1.png", { 250,150 }, { 280, 300.f }, "", "ButtonBG");
	m_Templete.pButtonEffect->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level","UI_MenuEffect2.png");
	m_Templete.pButtonEffect->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level","UI_MenuEffect3.png");
}
  
CTarget_Texture* CCraftCard::MakePart(
	const string& texture,
	const _float2& size,
	const _float2& offset,
	const string& pass,
	const string& name)
{
	auto pObj = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level("GamePlay_Level")
		.Build(name);

	auto pTex = dynamic_cast<CTarget_Texture*>(pObj);
	pTex->TargetSize(m_vTargetSize);

	if (!texture.empty())
		pTex->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", texture);

	if (!pass.empty())
		pTex->Get_Component<CSprite2D>()->ChangePass(pass);

	pTex->Set_Size(size);
	pTex->Center(offset);

	Get_Component<CObjectContainer>()->Add_Child(pTex, false);

	return pTex;
}

CUI_Text* CCraftCard::MakeText(const wstring& Text, const _float2& size, const _float& sclae, const _float2& offset,const _float4& color, const string& name)
{
	auto pObj = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_BaseText" })
		.Add_To_Level("GamePlay_Level")
		.Build(name);

	auto pTex = dynamic_cast<CUI_Text*>(pObj);

	if (!Text.empty())
		pTex->Set_Text(Text);

	pTex->Set_Scale(sclae);
	pTex->Set_Size(size);
	pTex->Set_Active(true);
	pTex->Set_Color(color);
	pTex->Set_Anchor(ANCHOR::Left | ANCHOR::Center);
	pTex->Align_To(ANCHOR::Left | ANCHOR::Center, offset);
	Get_Component<CObjectContainer>()->Add_Child(pTex, false);
	return pTex;
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

void CCraftCard::ResourceSlotsForCard::Render(ID3D11DeviceContext* pContext)
{
	pSourceItem->Render(pContext);
	pSourceItemLine->Render(pContext);
	pSourceItemBLine->Render(pContext);
	pSourceItemCountSlot->Render(pContext);
}

void CCraftCard::ResourceSlotsForCard::DeActive()
{
	pSourceItem->Get_Component<CSprite2D>()->Set_CompActive(false);
	pSourceItemLine->Get_Component<CSprite2D>()->Set_CompActive(false);
	pSourceItemBLine->Get_Component<CSprite2D>()->Set_CompActive(false);
	pSourceItemCountSlot->Get_Component<CSprite2D>()->Set_CompActive(false);
	pRecipeItemName->Get_Component<CTextSlot>()->Set_CompActive(false);
	pRecipeItemCount->Get_Component<CTextSlot>()->Set_CompActive(false);
	pRecipeItemNeed ->Get_Component<CTextSlot>()->Set_CompActive(false);
}

void CCraftCard::ResourceSlotsForCard::Active()
{
	pSourceItem->Get_Component<CSprite2D>()->Set_CompActive(true);
	pSourceItemLine->Get_Component<CSprite2D>()->Set_CompActive(true);
	pSourceItemBLine->Get_Component<CSprite2D>()->Set_CompActive(true);
	pSourceItemCountSlot->Get_Component<CSprite2D>()->Set_CompActive(true);
	pRecipeItemName->Get_Component<CTextSlot>()->Set_CompActive(true);
	pRecipeItemCount->Get_Component<CTextSlot>()->Set_CompActive(true);
	pRecipeItemNeed->Get_Component<CTextSlot>()->Set_CompActive(true);
}

void CCraftCard::BaseTemplete::Render(ID3D11DeviceContext* pContext)
{
	pBaseBackGround->Render(pContext);
	pImageBackGround->Render(pContext);
	pInfoBackGround->Render(pContext);
	pNameStroke->Render(pContext);
	pButtonBG->Render(pContext);
	pButton->Render(pContext);
	pButtonEffect->Render(pContext);
}
