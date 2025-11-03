#include "Client_Defines.h"
#include "UI_ItemIcon.h"
#include "Sprite2D.h"

CUI_ItemIcon::CUI_ItemIcon()
{
}

CUI_ItemIcon::CUI_ItemIcon(const CUI_ItemIcon& rhs)
	:CUI_Object(rhs)
{
}

HRESULT CUI_ItemIcon::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUI_ItemIcon::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Set_CompActive(true);
	return S_OK;
}

void CUI_ItemIcon::Priority_Update(_float dt)
{
}

void CUI_ItemIcon::Update(_float dt)
{
}

void CUI_ItemIcon::Late_Update(_float dt)
{
}

CUI_ItemIcon* CUI_ItemIcon::Create()
{
	CUI_ItemIcon* instance = new CUI_ItemIcon();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CUI_ItemIcon");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CUI_ItemIcon::Clone(INIT_DESC* pArg)
{
	CUI_ItemIcon* instance = new CUI_ItemIcon(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_ItemIcon");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_ItemIcon::Free()
{
	__super::Free();
}