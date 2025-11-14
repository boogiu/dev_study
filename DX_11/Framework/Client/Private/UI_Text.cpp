#include "Client_Defines.h"
#include "UI_Text.h"
#include "TextSlot.h"
#include "Sprite2D.h"`
CUI_Text::CUI_Text()
{
}

CUI_Text::CUI_Text(const CUI_Text& rhs)
	:CUI_Object(rhs)
{
}

CUI_Text::~CUI_Text()
{
}

HRESULT CUI_Text::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUI_Text::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	Add_Component<CTextSlot>();
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CTextSlot>()->Set_Color(_float4(0.447, 0.365, 0.259, 1.0));
	Get_Component<CTextSlot>()->Set_Font("Sindy");
	Get_Component<CTextSlot>()->Set_Size(0.7);
	Set_Anchor(ANCHOR::Left);

	return S_OK;
}

void CUI_Text::Priority_Update(_float dt)
{
}

void CUI_Text::Update(_float dt)
{
	if (m_bActive) {
		Get_Component<CTextSlot>()->Set_Anchor(m_eAnchor, { m_fWorldX-m_fSizeX*0.5f,m_fWorldY - m_fSizeY * 0.5f });
		Get_Component<CTextSlot>()->Push_Text();
	}
}

void CUI_Text::Late_Update(_float dt)
{
}

void CUI_Text::Render_GUI()
{
}

void CUI_Text::Set_Scale(_float size)
{
	Get_Component<CTextSlot>()->Set_Size( size);

}

_float CUI_Text::Text_Length()
{
	return Get_Component<CTextSlot>()->Get_TextSize();
}

void CUI_Text::Clear_Text()
{
	Get_Component<CTextSlot>()->Set_Text(L"");
}

void CUI_Text::Set_Text(const wstring& text)
{
	Get_Component<CTextSlot>()->Set_Text(text);
}

void CUI_Text::Rotate(_float radian)
{
	Get_Component<CTextSlot>()->Set_Rotate(radian);
}

void CUI_Text::Set_Color(_float4 color)
{
	Get_Component<CTextSlot>()->Set_Color(color);
}

void CUI_Text::UI_Active(void* pArg)
{
	m_bActive = true;
}

void CUI_Text::UI_DeActive(void* pArg)
{
	m_bActive = false;
}

CUI_Text* CUI_Text::Create()
{
	CUI_Text* instance = new CUI_Text();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CUI_Text");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CUI_Text::Clone(INIT_DESC* pArg)
{
	CUI_Text* instance = new CUI_Text(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_Text");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_Text::Free()
{
	__super::Free();
}
