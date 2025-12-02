#include "Client_Defines.h"
#include "UI_InvenSlot.h"
#include "Sprite2D.h"
#include "ObjectContainer.h"
#include "GameInstance.h"
#include "IUI_Service.h"
#include "UI_ItemText.h"
#include "TextSlot.h"
#include "UI_Text.h"
CUI_InvenSlot::CUI_InvenSlot()
{
}

CUI_InvenSlot::CUI_InvenSlot(const CUI_InvenSlot& rhs)
	:CUI_Object(rhs)
{
}

CUI_InvenSlot::~CUI_InvenSlot()
{
}

HRESULT CUI_InvenSlot::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUI_InvenSlot::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture(G_GlobalLevelKey, "UI_EmptySlot.png");
	Get_Component<CSprite2D>()->Add_Texture(G_GlobalLevelKey, "UI_SelecteSlot.png");
	Get_Component<CSprite2D>()->Set_CompActive(false);

	m_pIcon = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_ItemIcon" })
		.Add_To_Level("GamePlay_Level")
		.Build("Icon");

	CUI_Object *pObj = Builder::Create_UIObject({"GamePlay_Level", "GamePlay_GameObject_UI_ItemText"})
		.Add_To_Level("GamePlay_Level")
		.Position({0,-55})
		.Build("Icon");

	m_pText = dynamic_cast<CUI_ItemText*>(pObj);

	Add_Component<CObjectContainer>()->Add_Child(m_pIcon, false);
	Add_Component<CObjectContainer>()->Add_Child(m_pText, false); /*표시 */
	m_pText->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_ItemText.png");
	m_pText->Get_Component<CSprite2D>()->Set_CompActive(false);


	CUI_Object* pCount = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_BaseText" })
		.Add_To_Level("GamePlay_Level")
		.Position({5,15})
		.Scale({ 12,12 })
		.Build("Text");

	Get_Component<CObjectContainer>()->Add_Child(pCount, false);
	m_pCount = dynamic_cast<CUI_Text*>(pCount);
	m_pCount->Get_Component<CTextSlot>()->Set_Color(Brown);
	m_pCount->Get_Component<CTextSlot>()->Set_Font("Sindy");
	m_pCount->Get_Component<CTextSlot>()->Set_Size(0.7);
	m_pCount->Get_Component<CTextSlot>()->Set_OutLine(1.f, Ibory);
	return S_OK;
}

void CUI_InvenSlot::Priority_Update(_float dt)
{
	SizeControl(dt);
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CUI_InvenSlot::Update(_float dt)
{

	if (m_itemData.itemCount >1 ) {
		m_pCount->Set_Active(true);
		m_pCount->Get_Component<CTextSlot>()->Set_Text(to_wstring(m_itemData.itemCount));
	}
	else
	{
		m_pCount->Set_Active(false);
	}

	if (m_itemData.itemCount == 0) {
		m_itemData = {};
		m_pIcon->Get_Component<CSprite2D>()->Set_CompActive(false);
		m_pText->Get_Component<CSprite2D>()->Set_CompActive(false);
		m_pText->Get_Component<CTextSlot>()->Set_CompActive(false);
		m_pCount->Set_Active(false);
	}
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}
void CUI_InvenSlot::Active()
{
	m_pCount->Set_Active(true);
	m_pText->Get_Component<CSprite2D>()->Set_CompActive(true);
	m_pText->Get_Component<CTextSlot>()->Set_CompActive(true);
	m_pIcon->Get_Component<CSprite2D>()->Set_CompActive(true);
	Get_Component<CSprite2D>()->Set_CompActive(true);
}

void CUI_InvenSlot::DeActive()
{
	m_pCount->Set_Active(false);
	m_pText->Get_Component<CSprite2D>()->Set_CompActive(false);
	m_pText->Get_Component<CTextSlot>()->Set_CompActive(false);
	m_pIcon->Get_Component<CSprite2D>()->Set_CompActive(false);
	Get_Component<CSprite2D>()->Set_CompActive(false);
}

void CUI_InvenSlot::Late_Update(_float dt){
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CUI_InvenSlot::Render_GUI()
{
	__super::Render_GUI();
}


_bool CUI_InvenSlot::isAbleToContain(ITEM_DATA_DESC desc)
{
	if (m_itemData.itemCount == 0)
		return true;

	else {
		if (desc.FileName == m_itemData.itemData.FileName) {
			if (m_itemData.itemCount < desc.fullCount)
				return true;
			else
				return false;
		}
		else {
			return false;
		}
	}
	return false;
}

HRESULT CUI_InvenSlot::Add_Data(ITEM_DATA_DESC desc)
{
	if (m_itemData.itemCount == 0) {
		m_itemData.itemData = desc;
		m_itemData.itemCount = 1;
		m_pIcon->Get_Component<CSprite2D>()->Set_CompActive(true);
		m_pIcon->Set_CenterPos({ 0,0 });
		m_pIcon->Get_Component<CSprite2D>()->Change_Texture(0,"GamePlay_Level", desc.IconName);
	}
	else {
		m_itemData.itemCount += 1;
	}

	return S_OK;
}

void CUI_InvenSlot::PullOut_Data()
{
	/*1개 이상이면 개수 깎음*/
	if (m_itemData.itemCount <= 0) {
		m_itemData.itemCount = 0;
		return;
	}

	m_itemData.itemCount--;

	if (m_itemData.itemCount == 0) {
		m_itemData = {};
		/*빠지고도 살아있음*/
		m_pText->Get_Component<CSprite2D>()->Set_CompActive(false);
		m_pIcon->Get_Component<CSprite2D>()->Set_CompActive(false);
		m_pCount->Set_Active(false);
	}
}

void CUI_InvenSlot::SizeControl(_float dt)
{
	_vector size = {};
	_float2 curSize = { m_fSizeX, m_fSizeY };
	_float2 vSize;

	switch (m_eState)
	{
	case CUI_InvenSlot::Hovered:
		size = XMVectorLerp(XMLoadFloat2(&curSize), XMLoadFloat2(&m_vHoveredSize), dt * 15);
		XMStoreFloat2(&vSize, size);
		if(m_isValid)
			Get_Component<CSprite2D>()->ChangeSprite(1);
		if (m_itemData.itemCount != 0) {
			m_pText->Set_Text(m_itemData.itemData.ItemName, true);
		}
		break;
	case CUI_InvenSlot::HoverOut:
		size = XMVectorLerp(XMLoadFloat2(&curSize), XMLoadFloat2(&m_vBaseSize), dt * 15);
		XMStoreFloat2(&vSize, size);
		Get_Component<CSprite2D>()->ChangeSprite(0);
		m_pText->DeActive_Text();
		break;
	case CUI_InvenSlot::IDLE:
		vSize = m_vBaseSize;
		break;
	default:
		break;
	}

	m_fSizeX = vSize.x;
	m_fSizeY = vSize.y;

	m_pIcon->Set_Size(_vector{ m_fSizeX+5 ,m_fSizeY+5 });
}

CUI_InvenSlot* CUI_InvenSlot::Create()
{
	CUI_InvenSlot* instance = new CUI_InvenSlot();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CUI_EmptySlot");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CUI_InvenSlot::Clone(INIT_DESC* pArg)
{
	CUI_InvenSlot* instance = new CUI_InvenSlot(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_EmptySlot");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_InvenSlot::Free()
{
	__super::Free();
}