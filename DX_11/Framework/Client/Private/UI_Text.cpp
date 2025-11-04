#include "Client_Defines.h"
#include "UI_Text.h"
#include "TextSlot.h"
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

	return S_OK;
}

void CUI_Text::Priority_Update(_float dt)
{
}

void CUI_Text::Update(_float dt)
{
	if (m_bActive) {
		Get_Component<CTextSlot>()->Set_Anchor(m_eAnchor, { m_fWorldX,m_fWorldY });
		Get_Component<CTextSlot>()->Push_Text();
	}
}

void CUI_Text::Late_Update(_float dt)
{
}

void CUI_Text::Render_GUI()
{
}

_float CUI_Text::Text_Length()
{
	return Get_Component<CTextSlot>()->Get_TextSize();
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
