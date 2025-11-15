#include "Client_Defines.h"
#include "UI_ItemCard.h"
#include "Sprite2D.h"
#include "GameInstance.h"
#include "Texture.h"
#include "IResourceService.h"
#include "ObjectContainer.h"
#include "IRenderService.h"
#include "Target_Texture.h"
#include "IUI_Service.h"

CUI_ItemCard::CUI_ItemCard()
{
}

CUI_ItemCard::CUI_ItemCard(const CUI_ItemCard& rhs)
	:CUI_Object(rhs)
{
}

HRESULT CUI_ItemCard::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
	return S_OK;
}

HRESULT CUI_ItemCard::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	/*본인*/
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Set_CompActive(true);

	fBaseRadian = m_fRadian;
	m_InstanceName = "Card" + to_string(m_ObjectID);
	
	return S_OK;
}

void CUI_ItemCard::Awake()
{

	auto RenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	SHADER_PARAM textureParam{ RenderSys->Get_TargetSRV("CardTexture"), "Texture2D",0 };
	Get_Component<CSprite2D>()->Set_Param("SpriteTexture", textureParam);
}

void CUI_ItemCard::Priority_Update(_float dt)
{
	m_bHover = false;
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CUI_ItemCard::Update(_float dt)
{
	if (m_bHover) {
		Size_To({ m_CardSize.x * 1.2f,m_CardSize.y * 1.2f }, 8.f * dt);
		Rotate_To(fBaseRadian + XMConvertToRadians(10), 8.f * dt);
	}
	else {
		Size_To({ m_CardSize.x,m_CardSize.y }, 8.f * dt);
		Rotate_To(fBaseRadian, 8.f * dt);
	}

	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CUI_ItemCard::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);

}

void CUI_ItemCard::Render_GUI()
{
}

void CUI_ItemCard::UI_Active(void* pArg)
{
}

void CUI_ItemCard::UI_DeActive(void* pArg)
{
}
void CUI_ItemCard::Render_Card(ID3D11DeviceContext* pContext)
{
	D3D11_VIEWPORT oldVPs[8];
	UINT numVPs = 8;
	pContext->RSGetViewports(&numVPs, oldVPs);

	// 2. 카드 인덱스 기반 뷰포트 설정
	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = m_CardIndex.x * vBaseSize.x;
	vp.TopLeftY = m_CardIndex.y * vBaseSize.y;
	vp.Width = vBaseSize.x;
	vp.Height = vBaseSize.y;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;

	pContext->RSSetViewports(1, &vp);

	// 3. 카드의 내부 파츠 렌더
	m_pBaseBackGround->Render(pContext);
	m_pMaskedBackGround->Render(pContext);
	m_pIconImage->Render(pContext);

	// 4. 뷰포트 복구
	pContext->RSSetViewports(numVPs, oldVPs);

}

CTarget_Texture* CUI_ItemCard::Make_Part(string textureKey, const _float2& size, const _float2& pos, const string& passKey, const string& levelKey)
{

	CUI_Object* pUIObj = Builder::Create_UIObject({ levelKey, "GamePlay_GameObject_UI_Target_Texture" })
		.Add_To_Level(levelKey)
		.Build("InsideCard");

	auto pTarget = dynamic_cast<CTarget_Texture*>(pUIObj);

	pTarget->TargetSize(vBaseSize);

	auto pSprite = pTarget->Get_Component<CSprite2D>();

	if (!textureKey.empty())
		pSprite->Add_Texture(levelKey, textureKey);

	if (!passKey.empty())
		pSprite->ChangePass(passKey);

	pTarget->Set_Size(_vector{ size.x, size.y });

	pTarget->Center({ 0 ,0 });
	Get_Component<CObjectContainer>()->Add_Child(pTarget, false);

	return pTarget;
}

void CUI_ItemCard::Hover()
{
	m_bHover = true;
}

void CUI_ItemCard::Set_Data(const string& recipeImage, _bool CanCraft, wstring name)
{
	m_pIconImage->Get_Component<CSprite2D>()->Change_Texture(0, "GamePlay_Level", recipeImage);
	this->name = name;
	this->CanCraft = CanCraft;
}

wstring CUI_ItemCard::Get_Data()
{
	return name;
}

void CUI_ItemCard::Ready_Part()
{
	m_CardSize = vBaseSize;
	auto RcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	_float2 total = m_TargetSize;
	//5,3

	_float2 centerPos = {0,0};
	m_pBaseBackGround = Make_Part("UI_CardBackGruond.png", vBaseSize, centerPos, "", "GamePlay_Level");
	m_pMaskedBackGround = Make_Part("UI_CardInnerBack.png", { vBaseSize.x - 10.f,vBaseSize.y - 10.f, }, centerPos, "", "GamePlay_Level");
	m_pIconImage = Make_Part("MenuLayout_FtrWoodPile.png", { 80.f,80.f }, centerPos, "", "GamePlay_Level");

	SHADER_PARAM maskParam{ RcsMgr->Load_Texture("GamePlay_Level","UI_CardPattern.png")->Get_SRV(), "Texture2D",0 };
	m_pMaskedBackGround->Get_Component<CSprite2D>()->Set_Param("UI_MaskTexture", maskParam);
	m_pMaskedBackGround->Get_Component<CSprite2D>()->ChangePass("Masking_UI");

	scale = _float2(1.f / m_ColRow.x, 1.f / m_ColRow.y);
	offset = _float2(m_CardIndex.x / m_ColRow.x, m_CardIndex.y / m_ColRow.y);

 	SHADER_PARAM offsetParam{ &offset, "float2",sizeof(_float2) };
 	SHADER_PARAM scaleParam{ &scale, "float2",sizeof(_float2) };

	Get_Component<CSprite2D>()->Set_Param("vOffset_Tile", offsetParam);
	Get_Component<CSprite2D>()->Set_Param("vScale_Tile", scaleParam);
	Get_Component<CSprite2D>()->ChangePass("Tile_Texture");
}

CUI_ItemCard* CUI_ItemCard::Create()
{
	CUI_ItemCard* instance = new CUI_ItemCard();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CUI_ItemCard");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CUI_ItemCard::Clone(INIT_DESC* pArg)
{
	CUI_ItemCard* instance = new CUI_ItemCard(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_ItemCard");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_ItemCard::Free()
{
	__super::Free();
}
