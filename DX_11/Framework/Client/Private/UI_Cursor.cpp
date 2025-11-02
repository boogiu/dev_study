#include "Client_Defines.h"
#include "UI_Cursor.h"
#include "Sprite2D.h"
CUI_Cursor::CUI_Cursor()
{
}

CUI_Cursor::CUI_Cursor(const CUI_Cursor& rhs)
	:CUI_Object(rhs)
{
}

CUI_Cursor::~CUI_Cursor()
{
}

HRESULT CUI_Cursor::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUI_Cursor::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture(G_GlobalLevelKey, "UI_Point_Select.png");
	Get_Component<CSprite2D>()->Set_CompActive(false);

	return S_OK;
}

void CUI_Cursor::Priority_Update(_float dt)
{
}

void CUI_Cursor::Update(_float dt)
{
}

void CUI_Cursor::Late_Update(_float dt)
{
}

void CUI_Cursor::Render_GUI()
{
}

CUI_Cursor* CUI_Cursor::Create()
{
	CUI_Cursor* instance = new CUI_Cursor();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CUI_Cursor");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CUI_Cursor::Clone(INIT_DESC* pArg)
{
	CUI_Cursor* instance = new CUI_Cursor(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_Cursor");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_Cursor::Free()
{
	__super::Free();
}