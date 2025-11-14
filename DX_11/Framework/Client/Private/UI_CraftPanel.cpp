#include "Client_Defines.h"
#include "UI_CraftPanel.h"
#include "Sprite2D.h"
#include "GameInstance.h"
#include "ObjectContainer.h"

#include "UI_Text.h"
#include "TextSlot.h"
#include "UI_Cursor.h"
#include "UI_NameTag.h"
#include "SelectPanel.h"
#include "CraftCard.h"

#include "Level.h"
#include "DialogueManager.h"
#include "NonPlayer.h"
#include "UI_ItemCard.h"

#include "Helper_Func.h"
#include "IResourceService.h"
#include "Texture.h"
#include "UI_ItemText.h"
CUI_CraftPanel::CUI_CraftPanel()
{
}

CUI_CraftPanel::CUI_CraftPanel(const CUI_CraftPanel& rhs)
	:CUI_Object(rhs)
{
}

HRESULT CUI_CraftPanel::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUI_CraftPanel::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	Add_Component<CObjectContainer>();

	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_Craft_BackGround.png");
	auto pGradTexture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture("GamePlay_Level", "UI_Gradation.png");

	Get_Component<CSprite2D>()->ChangePass("Gradtion");
	SHADER_PARAM gradParam = { pGradTexture->Get_SRV(),"Texture2D",0 };
	Get_Component<CSprite2D>()->Set_Param("UI_GradationTexture", gradParam);

	Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_Gradation.png");

	Ready_Parts();

	auto Level = CGameInstance::GetInstance()->Get_CurrentLevel();

	m_bActive = false;
	m_pCraftCard->UI_DeActive(nullptr);
	for (size_t i = 0; i < m_pCards.size(); i++)
	{
		m_pCards[i]->UI_DeActive(nullptr);
	}
	Get_Component<CSprite2D>()->Set_CompActive(m_bActive);

	return S_OK;
}

void CUI_CraftPanel::Priority_Update(_float dt)
{
	Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	if (!m_bActive) return;

	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
	if (m_eState == Closed)
	{
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_SPACE)) 
			m_eState = Opened;
	}
	else if (m_eState == Opened) {
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_RIGHT)) {
			Valid_Index(1);
		}
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_LEFT)) {
			Valid_Index(-1);
		}
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_UP)) {
			Valid_Index(-m_Col);
		}
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_DOWN)) {
			Valid_Index(m_Col);
		}
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_SPACE)) {
			m_eState = Selected;
			m_pCraftCard->UI_Active(nullptr);
		}
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_SHIFT)) {
			m_eState = Closed;
			UI_DeActive(nullptr);
		}
	}
	else if (m_eState == Selected) {
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_SHIFT)) {
			m_eState = Opened;
			m_pCraftCard->UI_DeActive(nullptr);
		}
	}
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CUI_CraftPanel::Update(_float dt)
{
	//if (!m_bActive) return;
	m_pCursor->Set_Pivot(m_pCards[m_NowIndex]->Get_CenterPos(), { xCardSize * 0.5f,yCardSize * 0.2f }, { .5f, .5f });
	m_pCards[m_NowIndex]->Hover();
	m_pText->Set_Text(L"돌", true);
	m_pText->Set_CenterPos(m_pCards[m_NowIndex]->Local_CT(0.f - 20.f));
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CUI_CraftPanel::Late_Update(_float dt)
{
	//if (!m_bActive) return;
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CUI_CraftPanel::Render_GUI()
{

}

void CUI_CraftPanel::UI_Active(void* pArg)
{
	CRAFT_DATA_DESC*  pDesc = static_cast<CRAFT_DATA_DESC*>(pArg);
	m_OnClose = pDesc->OnClose;
	m_InvenData = pDesc->InvenData;

	m_bActive = true;
	m_pText->Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	for (size_t i = 0; i < m_pCards.size(); i++)
	{
		m_pCards[i]->UI_Active(pArg);
	}
}

void CUI_CraftPanel::UI_DeActive(void* pArg)
{
	m_bActive = false;

	m_pText->Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	m_pCraftCard->UI_DeActive(pArg);


	CRAFT_RESULT result = {};
	if(m_OnClose)
			m_OnClose(result);

	m_InvenData.clear();
}

void CUI_CraftPanel::Ready_Parts()
{
	xCardSize = 512 * 0.3f;
	yCardSize = 749 * 0.3f;

	_float xPadding = xCardSize * 0.5;
	_float yPadding = yCardSize * 0.3;
	_float xStart = 0 - m_fSizeX * 0.5f + xCardSize;
	_float yStart = 0 - m_fSizeY * 0.5f + yCardSize * .5f + yPadding;

	for (size_t i = 0; i < m_Row; i++)
	{
		for (size_t j = 0; j < m_Col; j++)
		{
			_uint index = i * m_Col + j;
			if (index >= m_CardCount)
				continue;

			CUI_Object* pItemCard = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_ItemCardl" })
				.Add_To_Level("GamePlay_Level")
				.Scale({ xCardSize ,yCardSize })
				.Position({
					xStart + (j * (xCardSize + xPadding)) ,
					yStart + (i * (yCardSize + yPadding)) })
					.Rotate(XMConvertToRadians(Helper::Get_Random_Float(-15.f, 15.f)))
				.Build("ItemCard");

			Get_Component<CObjectContainer>()->Add_Child(pItemCard, false);
			m_pCards.push_back(dynamic_cast<CUI_ItemCard*>(pItemCard));
		}
	}

	/*커서*/
	CUI_Object* pCursor = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Cursor" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 80,80 })
		.Position({ 0, 0 })
		.Build("Dialcursor");

	m_pCursor = dynamic_cast<CUI_Cursor*>(pCursor);
	Get_Component<CObjectContainer>()->Add_Child(m_pCursor, false);
	m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(true);

	/*카드*/
	CUI_Object* pCraftCard = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_CraftCard" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ m_fSizeX * 0.95f,m_fSizeY * 0.95f })
		.Position({ 0, 0 })
		.Build("CraftCard");

	m_pCraftCard = dynamic_cast<CCraftCard*>(pCraftCard);
	Get_Component<CObjectContainer>()->Add_Child(m_pCraftCard, false);
	m_pCraftCard->Get_Component<CSprite2D>()->Set_CompActive(true);


	CUI_Object* pObj = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_ItemText" })
		.Add_To_Level("GamePlay_Level")
		.Position({ 0,-55 })
		.Build("Icon");

	m_pText = dynamic_cast<CUI_ItemText*>(pObj);
	Add_Component<CObjectContainer>()->Add_Child(m_pText, false); /*표시 */
	m_pText->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_ItemText.png");
	m_pText->Get_Component<CSprite2D>()->Set_CompActive(false);
}


_int CUI_CraftPanel::Valid_Index(_int Add)
{
	_int next = m_NowIndex + Add;

	if (next < 0)
		next = 0;

	if (next >= m_CardCount)
		next = m_CardCount - 1;

	m_NowIndex = next;
	return m_NowIndex;
}

HRESULT CUI_CraftPanel::Read_CraftData()
{
	return E_NOTIMPL;
}



CUI_CraftPanel* CUI_CraftPanel::Create()
{
	CUI_CraftPanel* instance = new CUI_CraftPanel();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CUI_CraftPanel");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CUI_CraftPanel::Clone(INIT_DESC* pArg)
{
	CUI_CraftPanel* instance = new CUI_CraftPanel(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_CraftPanel");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_CraftPanel::Free()
{
	__super::Free();
}

