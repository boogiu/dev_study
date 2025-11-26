#include "Client_Defines.h"
#include "Loading_Island.h"
#include "Sprite2D.h"
#include "ObjectContainer.h"
#include "Builder.h"

CLoading_Island::CLoading_Island()
{
}

CLoading_Island::CLoading_Island(const CLoading_Island& rhs)
	:CUI_Object(rhs)
{
}

CLoading_Island::~CLoading_Island()
{
}

HRESULT CLoading_Island::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
    return S_OK;
}

HRESULT CLoading_Island::Initialize(INIT_DESC* pArg)
{
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture(G_GlobalLevelKey, "UI_LoadingIsland.png");

	//Builder::Create_UIObject();
	//Get_Component<CObjectContainer>()->

	__super::Initialize(pArg);
	return S_OK;
}

void CLoading_Island::Priority_Update(_float dt)
{
}

void CLoading_Island::Update(_float dt)
{
	m_fLifeTime += dt*2;
	m_fRadian = XMConvertToRadians(sinf(m_fLifeTime)*5.f);
	m_fLocalY += sinf(m_fLifeTime) * .2f;
}

void CLoading_Island::Late_Update(_float dt)
{
}

void CLoading_Island::Render_GUI()
{
	__super::Render_GUI();
}

void CLoading_Island::UI_Active(void* pArg)
{
	Get_Component<CSprite2D>()->Set_CompActive(true);
}

void CLoading_Island::UI_DeActive(void* pArg)
{
	Get_Component<CSprite2D>()->Set_CompActive(false);
}

CLoading_Island* CLoading_Island::Create()
{
	CLoading_Island* instance = new CLoading_Island();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CLoading_Island");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CLoading_Island::Clone(INIT_DESC* pArg)
{
	CLoading_Island* instance = new CLoading_Island(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CLoading_Island");
		Safe_Release(instance);
	}

	return instance;
}
void CLoading_Island::Free()
{
	__super::Free();
}
