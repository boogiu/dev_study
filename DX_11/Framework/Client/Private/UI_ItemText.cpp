#include "Client_Defines.h"
#include "UI_ItemText.h"
#include "Sprite2D.h"
#include "GameInstance.h"
#include "IFontService.h"
#include "TextSlot.h"

CUI_ItemText::CUI_ItemText()
{
}

CUI_ItemText::CUI_ItemText(const CUI_ItemText& rhs)
	:CUI_Object(rhs)
{
}

HRESULT CUI_ItemText::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUI_ItemText::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Set_CompActive(true);

	//m_info.FontTag = "Sindy";
	//m_info.Origin = { 0,0 };
	//m_info.Rotation = { 0 };
	//m_info.TextColor = { 1.f,1.f,1.f,1.f };

	Add_Component<CTextSlot>()->Set_CompActive(false);
	Get_Component<CTextSlot>()->Set_Font("Sindy");
	Get_Component<CTextSlot>()->Set_Size(0.7f);
	Get_Component<CTextSlot>()->Set_Color({ 1.0f, 0.984f, 0.905f ,1.f });
	//Get_Component<CTextSlot>()->Set_AutoPos(ANCHOR::Center, { m_fWorldX,m_fWorldY });
	m_Text = L"µ¹";
	m_fSizeX = 0;
	m_fSizeY = 0;
	return S_OK;
}

void CUI_ItemText::Priority_Update(_float dt)
{
}

void CUI_ItemText::Update(_float dt)
{

	if (m_eState == state::Idle) {
		Get_Component<CTextSlot>()->Set_Anchor(ANCHOR::Center, { m_fWorldX,m_fWorldY });
		Get_Component<CTextSlot>()->Push_Text();
	}
	if (m_eState == Opened) {
		m_fOpenTime += dt * 6;
		_float2 CurSize = { m_fSizeX, m_fSizeY };
		_vector LerpSize = XMVectorLerp(XMLoadFloat2(&CurSize), XMLoadFloat2(&m_BaseSize), m_fOpenTime);
		Set_Size(LerpSize);

		if (m_fOpenTime > 1.f) {
			Get_Component<CTextSlot>()->Set_CompActive(true);
			Get_Component<CSprite2D>()->Set_CompActive(true);
			m_eState = Idle;
			m_fOpenTime = 0;
		}
	}
}

void CUI_ItemText::Late_Update(_float dt)
{
}

void CUI_ItemText::Set_Text(wstring Text, _bool Active)
{
	m_Text = Text;
	if (m_eState == Closed)
		m_eState = Opened;
	Get_Component<CTextSlot>()->Set_Text(Text);
}

void CUI_ItemText::DeActive_Text()
{
	m_eState = Closed;
	m_fSizeX = 0;
	m_fSizeY = 0;

	Get_Component<CSprite2D>()->Set_CompActive(false);
	Get_Component<CTextSlot>()->Set_CompActive(false);
}

CUI_ItemText* CUI_ItemText::Create()
{
	CUI_ItemText* instance = new CUI_ItemText();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CUI_ItemText");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CUI_ItemText::Clone(INIT_DESC* pArg)
{
	CUI_ItemText* instance = new CUI_ItemText(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_ItemText");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_ItemText::Free()
{
	__super::Free();
}