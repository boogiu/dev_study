#include "Client_Defines.h"
#include "Target_Text.h"

#include "TextSlot.h"
#include "Sprite2D.h"`

CTarget_Text::CTarget_Text()
{
}

CTarget_Text::CTarget_Text(const CTarget_Text& rhs)
	:CUI_Object(rhs)
{
}

CTarget_Text::~CTarget_Text()
{
}

HRESULT CTarget_Text::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CTarget_Text::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	Add_Component<CTextSlot>();
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CTextSlot>()->Set_Color(_float4(0.447, 0.365, 0.259, 1.0));
	Get_Component<CTextSlot>()->Set_Font("Sindy");
	Get_Component<CTextSlot>()->Set_Size(0.7);

	return S_OK;
}

void CTarget_Text::Priority_Update(_float dt)
{
}

void CTarget_Text::Update(_float dt)
{
}

void CTarget_Text::Late_Update(_float dt)
{
}

void CTarget_Text::Render_GUI()
{
}

void CTarget_Text::Render(ID3D11DeviceContext* pContext)
{
	Get_Component<CTextSlot>()->Render_Text();
}

void CTarget_Text::UI_Active(void* pArg)
{
}

void CTarget_Text::UI_DeActive(void* pArg)
{
}

_float CTarget_Text::Text_Length()
{
	return Get_Component<CTextSlot>()->Get_TextSize();
}

void CTarget_Text::Set_Text(const wstring& text)
{
	Get_Component<CTextSlot>()->Set_Text(text);
}

void CTarget_Text::Clear_Text()
{
	Get_Component<CTextSlot>()->Set_Text(L"");
}

void CTarget_Text::Set_Color(_float4 color)
{
	Get_Component<CTextSlot>()->Set_Color(color);
}

void CTarget_Text::Set_Scale(_float size)
{
	Get_Component<CTextSlot>()->Set_Size(size);
}

void CTarget_Text::Rotate(_float radian)
{
	Get_Component<CTextSlot>()->Set_Rotate(radian);
}

void CTarget_Text::TargetSize(_float2 size)
{
	m_WinSizeX = size.x;
	m_WinSizeY = size.y;
}

void CTarget_Text::Center(_float2 offset)
{
	m_fLocalX = m_WinSizeX * 0.5f + offset.x;
	m_fLocalY = m_WinSizeY * 0.5f + offset.y;
}

_float2 CTarget_Text::Target_Center(_float2 offset)
{
	return _float2(m_fLocalX, m_fLocalY);
}

_float CTarget_Text::Target_L(_float offset)
{
	return m_fLocalX - m_fLocalX * 0.5f + offset;
}

_float CTarget_Text::Target_R(_float offset)
{
	return m_fLocalX - m_fLocalX * 0.5f + offset;
}

_float CTarget_Text::Target_B(_float offset)
{
	return m_fLocalY + m_fLocalY * 0.5f + offset;
}

_float CTarget_Text::Target_T(_float offset)
{
	return m_fLocalY - m_fLocalY * 0.5f + offset;
}

CTarget_Text* CTarget_Text::Create()
{
	CTarget_Text* instance = new CTarget_Text();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CTarget_Text");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CTarget_Text::Clone(INIT_DESC* pArg)
{
	CTarget_Text* instance = new CTarget_Text(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CTarget_Text");
		Safe_Release(instance);
	}

	return instance;
}

void CTarget_Text::Free()
{
	__super::Free();
}
