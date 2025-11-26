#include "Client_Defines.h"
#include "Loading_Background.h"
#include "Sprite2D.h"
#include "ObjectContainer.h"
#include "Builder.h"

CLoading_Background::CLoading_Background()
{
}

CLoading_Background::CLoading_Background(const CLoading_Background& rhs)
	:CUI_Object(rhs)
{
}

CLoading_Background::~CLoading_Background()
{
}

HRESULT CLoading_Background::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
    return S_OK;
}

HRESULT CLoading_Background::Initialize(INIT_DESC* pArg)
{
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture(G_GlobalLevelKey, "UI_BackGround.png");

	CUI_Object* pObj = Builder::Create_UIObject({ G_GlobalLevelKey,"GamePlay_UI_Loading_Island" })
		.Scale({ 200,200 }).Set_Anchor(ANCHOR::Right | ANCHOR::Bottom, { 640,330 })
		.Add_To_Level("Loading_Level").Build("LoadingIsland");
	Get_Component<CObjectContainer>()->Add_Child(pObj,false);

	//pObj = Builder::Create_UIObject({ G_GlobalLevelKey,"GamePlay_UI_Loading_Icon" })
	//	.Scale({ 200,50 }).Set_Anchor(ANCHOR::Right | ANCHOR::Bottom, { 640,330 })
	//	.Add_To_Level("Loading_Level").Build("LoadingIsland");
	//Get_Component<CObjectContainer>()->Add_Child(pObj,false);

	__super::Initialize(pArg);

	return S_OK;
}

void CLoading_Background::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CLoading_Background::Update(_float dt)
{
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CLoading_Background::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CLoading_Background::Render_GUI()
{
	__super::Render_GUI();
}

void CLoading_Background::UI_Active(void* pArg)
{
	Get_Component<CSprite2D>()->Set_CompActive(true);
}

void CLoading_Background::UI_DeActive(void* pArg)
{
	Get_Component<CSprite2D>()->Set_CompActive(false);
}

CLoading_Background* CLoading_Background::Create()
{
	CLoading_Background* instance = new CLoading_Background();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CLoading_Background");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CLoading_Background::Clone(INIT_DESC* pArg)
{
	CLoading_Background* instance = new CLoading_Background(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CLoading_Background");
		Safe_Release(instance);
	}

	return instance;
}
void CLoading_Background::Free()
{
	__super::Free();
}
