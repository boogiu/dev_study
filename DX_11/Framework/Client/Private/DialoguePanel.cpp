#include "Client_Defines.h"
#include "DialoguePanel.h"
#include "Sprite2D.h"
#include "GameInstance.h"
#include "ObjectContainer.h"
#include "UI_Text.h"
#include "TextSlot.h"
#include "UI_Cursor.h"

CDialoguePanel::CDialoguePanel()
{
}

CDialoguePanel::CDialoguePanel(const CDialoguePanel& rhs)
	:CUI_Object(rhs)
{
}

CDialoguePanel::~CDialoguePanel()
{

}
HRESULT CDialoguePanel::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CDialoguePanel::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Add_Component<CObjectContainer>();
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_DialogueBox.png");

	m_pTexts.resize(4, nullptr);
	return S_OK;
}

void CDialoguePanel::Priority_Update(_float dt)
{
	if (!m_bActive) return;
		Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CDialoguePanel::Update(_float dt)
{
	if (!m_bActive) return;
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CDialoguePanel::Late_Update(_float dt)
{
	if (!m_bActive) return;
		Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CDialoguePanel::Render_GUI()
{
	ImGui::Text("Debug");
}

CDialoguePanel* CDialoguePanel::Create()
{
	CDialoguePanel* instance = new CDialoguePanel();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CDialoguePanel");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CDialoguePanel::Clone(INIT_DESC* pArg)
{
	CDialoguePanel* instance = new CDialoguePanel(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CDialoguePanel");
		Safe_Release(instance);
	}

	return instance;
}

void CDialoguePanel::Free()
{
	__super::Free();
}

