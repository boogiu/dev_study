#include "Client_Defines.h"
#include "TexturePanel.h"
#include "Sprite2D.h"
CTexturePanel::CTexturePanel()
{
}

CTexturePanel::CTexturePanel(const CTexturePanel& rhs)
	:CUI_Object(rhs)
{
}

CTexturePanel::~CTexturePanel()
{
}

HRESULT CTexturePanel::Initialize_Prototype()
{
	__super::Initialize_Prototype();
    return S_OK;
}

HRESULT CTexturePanel::Initialize(INIT_DESC* pArg)
{
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	__super::Initialize(pArg);
	return S_OK;
}

void CTexturePanel::Priority_Update(_float dt)
{
}

void CTexturePanel::Update(_float dt)
{
}

void CTexturePanel::Late_Update(_float dt)
{
}

void CTexturePanel::Render_GUI()
{
}


CTexturePanel* CTexturePanel::Create()
{
	CTexturePanel* instance = new CTexturePanel();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CTexturePanel");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CTexturePanel::Clone(INIT_DESC* pArg)
{
	CTexturePanel* instance = new CTexturePanel(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CTexturePanel");
		Safe_Release(instance);
	}

	return instance;
}
void CTexturePanel::Free()
{
	__super::Free();
}
