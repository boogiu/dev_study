#include "Client_Defines.h"
#include "Sprite2D.h"
#include "SelectPanel.h"
#include "GameInstance.h"
#include "ObjectContainer.h"
#include "UI_Text.h"
#include "TextSlot.h"
#include "UI_Cursor.h"
#include "AudioSource.h"

CSelectPanel::CSelectPanel()
{
}

CSelectPanel::CSelectPanel(const CSelectPanel& rhs)
	:CUI_Object(rhs)
{
}

CSelectPanel::~CSelectPanel()
{

}
HRESULT CSelectPanel::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CAudioSource>();
	return S_OK;
}

HRESULT CSelectPanel::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Add_Component<CObjectContainer>();
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_SelectPanel.png");
	Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_SelectPanel2.png");
	Get_Component<CSprite2D>()->Set_CompActive(false);

	m_pSelectHighlight = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_TexturePanel" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 0,0 })
		.Position({ -20,0 })
		.Build("SeletcHighlight");

	m_pSelectHighlight->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_SelectedTag.png");
	m_pSelectHighlight->Get_Component<CSprite2D>()->Set_CompActive(false);
	m_pSelectHighlight->Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CObjectContainer>()->Add_Child(m_pSelectHighlight, false);


	m_pTexts.resize(4, nullptr);
	for (size_t i = 0; i < 4; i++)
	{
		CUI_Object* pUI = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_BaseText" })
			.Add_To_Level("GamePlay_Level")
			.Scale({ 200,50 })
			.Build("Text");
		Get_Component<CObjectContainer>()->Add_Child(pUI, false);
		m_pTexts[i] = dynamic_cast<CUI_Text*>(pUI);
		m_pTexts[i]->Get_Component<CTextSlot>()->Set_Color(_float4(0.447, 0.365, 0.259, 1.0));
		m_pTexts[i]->Get_Component<CTextSlot>()->Set_Font("Sindy");
		m_pTexts[i]->Get_Component<CTextSlot>()->Set_Size(0.7);
		m_pTexts[i]->Set_Anchor(ANCHOR::Left);
		m_pTexts[i]->Set_Active(false);
	}

	CUI_Object* pCursor = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Cursor" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 50,50 })
		.Position({ 0,0 })
		.Build("Cursor");

	m_pCursor = dynamic_cast<CUI_Cursor*>(pCursor);
	m_pCursor->Get_Component<CSprite2D>()->ChangeSprite(1);


	Get_Component<CObjectContainer>()->Add_Child(pCursor, false);
	Get_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "UI_Wrapping_Wrap_Basic.wav", "Open", false, SOUND_GROUP::UI);
	Get_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "UI_PocketMenu_Close.wav", "Close", false, SOUND_GROUP::UI);
	Get_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "UI_Decide.wav", "Select", false, SOUND_GROUP::UI);
	Get_Component<CAudioSource>()->Add_Slot("GamePlay_Level", "UI_Tab_L.wav", "Tab", false, SOUND_GROUP::UI);

	Get_Component<CAudioSource>()->Set_3DAttribute("Open", false);
	Get_Component<CAudioSource>()->Set_3DAttribute("Close", false);
	Get_Component<CAudioSource>()->Set_3DAttribute("Select", false);
	Get_Component<CAudioSource>()->Set_3DAttribute("Tab", false);
	return S_OK;
}

void CSelectPanel::Priority_Update(_float dt)
{
	if (!m_bActive) return;

	if (m_SelectCount > 1) {
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_DOWN)) {
			m_NowIndex++;
			Get_Component<CAudioSource>()->Play("Tab");
		}
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_UP)) {
			m_NowIndex--;
			Get_Component<CAudioSource>()->Play("Tab");
		}
		if (m_NowIndex > m_SelectCount - 1)
			m_NowIndex = m_SelectCount - 1;
		if (m_NowIndex < 0)
			m_NowIndex = 0;
	}
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_SPACE)) {
		m_SelectedIndex = m_NowIndex;
		Get_Component<CAudioSource>()->Play("Select");
	}
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CSelectPanel::Update(_float dt)
{
	if (!m_bActive) return;
	_bool Sizecomp = 
		Size_To({ 
		MaxWidth * 1.3f < 130 ? 130 : MaxWidth * 1.3f,
			40.f * m_SelectCount < 130 ? 130 : 40.f * m_SelectCount}, 15 * dt);

	if(!Sizecomp)
		m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(false);

	if (Sizecomp) {
		m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(true);
		m_pSelectHighlight->Get_Component<CSprite2D>()->Set_CompActive(true);

		for (size_t i = 0; i < m_SelectCount; i++)
		{
			m_pTexts[i]->Set_Active(true);
			m_pTexts[i]->Set_Size(_float2{ m_pTexts[i]->Text_Length(),20.f*0.7f });

			m_pTexts[i]->Align_To(ANCHOR::Left,
				{ (-m_fSizeX * 0.5f) + 25.f,
					(i - (m_SelectCount - 1) * 0.5f) * 30.f });
		}
		m_pSelectHighlight->Size_To({ m_pTexts[m_NowIndex]->Text_Length(),10 }, dt * 6);

		m_pSelectHighlight->Align_To(ANCHOR::Left,
			{ (-m_fSizeX * 0.5f) + 20.f,
			m_pTexts[m_NowIndex]->Local_Center().y });

		m_pCursor->Set_Pivot({ -m_fSizeX * 0.5f , m_pTexts[m_NowIndex]->Local_Center().y }, { 0,0 }, { 1,0 });

	}

	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CSelectPanel::Late_Update(_float dt)
{
	if (!m_bActive) return;
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CSelectPanel::Render_GUI()
{
}

void CSelectPanel::Active()
{
	m_bActive = true;
}

void CSelectPanel::DeActive()
{
	m_NowIndex = 0;
	m_SelectCount = 0;
	m_SelectedIndex = -1;
	Set_Size(_float2{ 0,0 });
	Get_Component<CSprite2D>()->Set_CompActive(false);
	m_pSelectHighlight->Set_Size(_float2{ 0,0 });
	m_bActive = false;
	MaxWidth = 0.f;
}

/*델타 타임 현재는 사용 안함*/
void CSelectPanel::Set_Selecte(vector<wstring> select, _float dt)
{
	if (select.empty())
		return;

	if (m_bActive)
		return;

	m_SelectCount = select.size();
	for (size_t i = 0; i < m_pTexts.size(); i++)
	{
		m_pTexts[i]->Set_Active(false);
		if (i >= m_SelectCount) {
			m_pTexts[i]->Get_Component<CTextSlot>()->Set_Text(L"");
			continue;
		}
		m_pTexts[i]->Get_Component<CTextSlot>()->Set_Text(select[i]);
		m_pTexts[i]->Set_Anchor(ANCHOR::Left | ANCHOR::Center);

		if (m_pTexts[i]->Text_Length() > MaxWidth)
			MaxWidth = m_pTexts[i]->Text_Length();
	}

}

CSelectPanel* CSelectPanel::Create()
{
	CSelectPanel* instance = new CSelectPanel();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CSelectPanel");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CSelectPanel::Clone(INIT_DESC* pArg)
{
	CSelectPanel* instance = new CSelectPanel(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CSelectPanel");
		Safe_Release(instance);
	}

	return instance;
}

void CSelectPanel::Free()
{
	__super::Free();
}

