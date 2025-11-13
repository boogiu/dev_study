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

	m_bActive = true;
	return S_OK;
}

void CUI_CraftPanel::Priority_Update(_float dt)
{
	if (!m_bActive) return;

	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
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
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CUI_CraftPanel::Update(_float dt)
{
	if (!m_bActive) return;
	m_pCursor->Set_Pivot(m_pCards[m_NowIndex]->Get_CenterPos(), { xCardSize * 0.5f,yCardSize * 0.2f }, { .5f, .5f });
	m_pCards[m_NowIndex]->Hover();
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CUI_CraftPanel::Late_Update(_float dt)
{
	if (!m_bActive) return;
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CUI_CraftPanel::Render_GUI()
{
	__super::Render_GUI();
#ifdef _USING_GUI

	auto& vector = Get_Children();
	for (size_t i = 0; i < vector.size(); i++)
	{
		if (ImGui::Button(string(vector[i]->Get_InstanceName() + ":" + to_string(i)).c_str())) {
			childIndex = i;
		}
	}
	ImGui::Begin("UI_PANEL");
	vector[childIndex]->Render_GUI();
	ImGui::End();
#endif // _USING_GUI
}

void CUI_CraftPanel::UI_Active(void* pArg)
{
	Get_Component<CSprite2D>()->Set_CompActive(true);
}

void CUI_CraftPanel::UI_DeActive(void* pArg)
{
	Get_Component<CSprite2D>()->Set_CompActive(false);

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
						.Rotate(XMConvertToRadians(Helper::Get_Random_Float(-5.f, 5.f)))
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

