#include "Client_Defines.h"
#include "UI_EmptySlot.h"
#include "Sprite2D.h"

CUI_EmptySlot::CUI_EmptySlot()
{
}

CUI_EmptySlot::CUI_EmptySlot(const CUI_EmptySlot& rhs)
	:CUI_Object(rhs)
{
}

CUI_EmptySlot::~CUI_EmptySlot()
{
}

HRESULT CUI_EmptySlot::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUI_EmptySlot::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture(G_GlobalLevelKey, "UI_EmptySlot.png");
	Get_Component<CSprite2D>()->Set_CompActive(false);

	return S_OK;
}

void CUI_EmptySlot::Priority_Update(_float dt)
{
	SizeControl(dt);
}

void CUI_EmptySlot::Update(_float dt)
{
}

void CUI_EmptySlot::Late_Update(_float dt)
{
}

void CUI_EmptySlot::Render_GUI()
{
	__super::Render_GUI();
}

void CUI_EmptySlot::SizeControl(_float dt)
{
	_vector size = {};
	_float2 curSize = { m_fSizeX, m_fSizeY };
	_float2 vSize;

	switch (m_eState)
	{
	case CUI_EmptySlot::Hovered:
		size = XMVectorLerp(XMLoadFloat2(&curSize), XMLoadFloat2(&m_vHoveredSize), dt * 8);
		XMStoreFloat2(&vSize, size);
		break;
	case CUI_EmptySlot::HoverOut:
		size = XMVectorLerp(XMLoadFloat2(&curSize), XMLoadFloat2(&m_vBaseSize), dt * 8);
		XMStoreFloat2(&vSize, size);
		break;
	case CUI_EmptySlot::IDLE:
		vSize = m_vBaseSize;
		break;
	default:
		break;
	}

	m_fSizeX = vSize.x;
	m_fSizeY = vSize.y;
}

CUI_EmptySlot* CUI_EmptySlot::Create()
{
	CUI_EmptySlot* instance = new CUI_EmptySlot();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CUI_EmptySlot");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CUI_EmptySlot::Clone(INIT_DESC* pArg)
{
	CUI_EmptySlot* instance = new CUI_EmptySlot(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_EmptySlot");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_EmptySlot::Free()
{
	__super::Free();
}