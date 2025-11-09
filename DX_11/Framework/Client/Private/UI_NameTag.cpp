#include "Client_Defines.h"
#include "Sprite2D.h"
#include "UI_NameTag.h"
#include "GameInstance.h"
#include "ObjectContainer.h"
#include "UI_Text.h"
#include "TextSlot.h"
#include "UI_Cursor.h"

CUI_NameTag::CUI_NameTag()
{
}

CUI_NameTag::CUI_NameTag(const CUI_NameTag& rhs)
	:CUI_Object(rhs)
{
}

CUI_NameTag::~CUI_NameTag()
{

}
HRESULT CUI_NameTag::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUI_NameTag::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Add_Component<CObjectContainer>();
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_DialogueNameTag.png");

	CUI_Object* pUI = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_BaseText" })
		.Add_To_Level("GamePlay_Level")
		.Position({ 0,3 })
		.Build("Text");

	Get_Component<CObjectContainer>()->Add_Child(pUI, false);
	m_pText = dynamic_cast<CUI_Text*>(pUI);
	m_pText->Set_Anchor(ANCHOR::Center);
	m_pText->Rotate(XMConvertToRadians(-8));
	m_pText->Set_Color(Ibory);
	m_pText->Set_Scale(0.8f);
	return S_OK;
}

void CUI_NameTag::Priority_Update(_float dt)
{
	if (!m_bActive) return;

	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CUI_NameTag::Update(_float dt)
{
	if (!m_bActive) return;

	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CUI_NameTag::Late_Update(_float dt)
{
	if (!m_bActive) return;
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CUI_NameTag::Render_GUI()
{
}

void CUI_NameTag::Set_Active(_bool active)
{
	m_bActive = active;
	Get_Component<CSprite2D>()->Set_CompActive(active);
	if (!active) {
		m_pText->Set_Active(false);
		m_pText->Clear_Text();
	}
	else {
		m_pText->Set_Active(true);
	}
}

void CUI_NameTag::Set_Name(const wstring& name)
{
	m_pText->Set_Text(name);
}

CUI_NameTag* CUI_NameTag::Create()
{
	CUI_NameTag* instance = new CUI_NameTag();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CUI_NameTag");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CUI_NameTag::Clone(INIT_DESC* pArg)
{
	CUI_NameTag* instance = new CUI_NameTag(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_NameTag");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_NameTag::Free()
{
	__super::Free();
}

