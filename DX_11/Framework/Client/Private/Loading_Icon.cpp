#include "Client_Defines.h"
#include "Loading_Icon.h"
#include "Sprite2D.h"
#include "ObjectContainer.h"
#include "Builder.h"

CLoading_Icon::CLoading_Icon()
{
}

CLoading_Icon::CLoading_Icon(const CLoading_Icon& rhs)
	:CUI_Object(rhs)
{
}

CLoading_Icon::~CLoading_Icon()
{
}

HRESULT CLoading_Icon::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
    return S_OK;
}

HRESULT CLoading_Icon::Initialize(INIT_DESC* pArg)
{
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture(G_GlobalLevelKey, "UI_MapLoadIcon.png");

	//Builder::Create_UIObject();
	//Get_Component<CObjectContainer>()->

	__super::Initialize(pArg);
	return S_OK;
}

void CLoading_Icon::Priority_Update(_float dt)
{
}

void CLoading_Icon::Update(_float dt)
{

}

void CLoading_Icon::Late_Update(_float dt)
{
}

void CLoading_Icon::Render_GUI()
{
	__super::Render_GUI();
}

void CLoading_Icon::UI_Active(void* pArg)
{
	Get_Component<CSprite2D>()->Set_CompActive(true);
}

void CLoading_Icon::UI_DeActive(void* pArg)
{
	Get_Component<CSprite2D>()->Set_CompActive(false);
}

CLoading_Icon* CLoading_Icon::Create()
{
	CLoading_Icon* instance = new CLoading_Icon();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CLoading_Icon");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CLoading_Icon::Clone(INIT_DESC* pArg)
{
	CLoading_Icon* instance = new CLoading_Icon(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CLoading_Icon");
		Safe_Release(instance);
	}

	return instance;
}
void CLoading_Icon::Free()
{
	__super::Free();
}
