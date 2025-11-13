#include "Client_Defines.h"
#include "CraftCard.h"

#include "Sprite2D.h"
#include "GameInstance.h"
#include "Texture.h"
#include "IResourceService.h"
#include "ObjectContainer.h"


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
	Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_BackgroundOverlay.png");

	Ready_Part();
	return S_OK;
}

void CCraftCard::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CCraftCard::Update(_float dt)
{
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CCraftCard::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CCraftCard::Render_GUI()
{
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

void CCraftCard::Ready_Part()
{
	_float startX = m_fLocalX - m_fSizeX * 0.5f;
	_float startY = m_fLocalY - m_fSizeY * 0.5f;
	_float endX = m_fLocalX + m_fSizeX * 0.5f;

	_float lineSizeX = { 500 };
	CUI_Object* pLineStroke = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_TexturePanel" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ lineSizeX,3 })
		.Position({ startX + lineSizeX * 0.5f + 80,startY + 120 })
		.Rotate(0)
		.Build("pLineStroke");

	pLineStroke->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_LineStroke.png");

	Get_Component<CObjectContainer>()->Add_Child(pLineStroke, false);

	CUI_Object* pBack = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_TexturePanel" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 500,500 })
		.Rotate(0)
		.Build("pBack");
	

	pBack->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_RecipeInfoBackground.png");
	pBack->Align_To(ANCHOR::Right, { endX- 80,0 });
	Get_Component<CObjectContainer>()->Add_Child(pBack, false);

	CUI_Object* pImageBack = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_TexturePanel" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 450,450 })
		.Rotate(0)
		.Build("pImageBack");
	auto pMaskTexture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture("GamePlay_Level", "UI_CraftIconMask.png");
	SHADER_PARAM maskParam = { pMaskTexture->Get_SRV(),"Texture2D",0 };
	pImageBack->Get_Component<CSprite2D>()->Set_Param("UI_MaskTexture", maskParam);
	pImageBack->Get_Component<CSprite2D>()->ChangePass("Inside_Empty");
	pImageBack->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_CraftIconImageMask.png");
	pImageBack->Align_To(ANCHOR::Left, { pLineStroke->LC().x,40 });

	Get_Component<CObjectContainer>()->Add_Child(pImageBack, false);


	CUI_Object* pImageIcon = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_TexturePanel" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 250,250 })
		.Rotate(0)
		.Build("pImageBack");
	pImageIcon->Align_To(ANCHOR::Center, pImageBack->Get_CenterPos());
	pImageIcon->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "MenuLayout_FtrWoodPile.png");

	Get_Component<CObjectContainer>()->Add_Child(pImageIcon, false);


	CUI_Object* pCatIcon = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_TexturePanel" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 50,50 })
		.Rotate(0)
		.Build("pCatIcon");
	pCatIcon->Align_To(ANCHOR::Bottom | ANCHOR::Left, pLineStroke->LT(0,-5));
	pCatIcon->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_CatIconBackground.png");
	auto pCatMaskTexture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture("GamePlay_Level", "UI_IconCatFurniture.png");
	SHADER_PARAM maskCatParam = { pCatMaskTexture->Get_SRV(),"Texture2D",0 };
	_float2 maskScale = { 0.7f,0.7f };
	SHADER_PARAM scaleCatParam = { &maskScale,"float2",sizeof(_float2)};
	pCatIcon->Get_Component<CSprite2D>()->Set_Param("UI_MaskTexture", maskCatParam);
	pCatIcon->Get_Component<CSprite2D>()->Set_Param("MaskScale", scaleCatParam);
	pCatIcon->Get_Component<CSprite2D>()->ChangePass("Inside_Empty");

	Get_Component<CObjectContainer>()->Add_Child(pCatIcon, false);
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