#include "Client_Defines.h"
#include "Sprite2D.h"
#include "SelectPanel.h"
#include "ObjectContainer.h"
#include "UI_Text.h"
#include "TextSlot.h"

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
	return S_OK;
}

HRESULT CSelectPanel::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Add_Component<CObjectContainer>();
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_SelectPanel.png");
	Get_Component<CSprite2D>()->Set_CompActive(false);

	m_pTexts.resize(4, nullptr);
	for (size_t i = 0; i < 4; i++)
	{
		CUI_Object* pUI = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_BaseText" })
			.Add_To_Level("GamePlay_Level")
			.Scale({200,50})
			.Build("Text");
		Get_Component<CObjectContainer>()->Add_Child(pUI, false);
		m_pTexts[i] = dynamic_cast<CUI_Text*>(pUI);
		m_pTexts[i]->Get_Component<CTextSlot>()->Set_Color(_float4(0.447, 0.365, 0.259, 1.0));
		m_pTexts[i]->Get_Component<CTextSlot>()->Set_Font("Sindy");
		m_pTexts[i]->Get_Component<CTextSlot>()->Set_Size(0.7);
	}

	return S_OK;
}

void CSelectPanel::Priority_Update(_float dt)
{
	if (!m_bActive) return;

		Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CSelectPanel::Update(_float dt)
{
	if (!m_bActive) return;

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
	Set_Size(_float2{ 0,0 });
	Get_Component<CSprite2D>()->Set_CompActive(false);
	m_bActive = false;
}

void CSelectPanel::Set_Selecte(vector<wstring> select)
{
	if (select.empty())
		return;

	if (m_bActive)
		return;

	_uint count = select.size();

	for (size_t i = 0; i < m_pTexts.size(); i++)
	{
		if (i >= count)
			return;
		m_pTexts[i]->Get_Component<CTextSlot>()->Set_Text(select[i]);
		m_pTexts[i]->Set_Active(true);
		m_pTexts[i]->Set_Anchor(ANCHOR::Left|ANCHOR::Center);
		/*사이즈 변경 된 이후에*/
		m_pTexts[i]->Set_CenterPos({
		m_fWorldX - m_fSizeX * 0.5f + 20.f,
		(i - count) * 25.f
			});
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

