#include "Client_Defines.h"
#include "UI_CraftPanel.h"
#include "Sprite2D.h"
#include "GameInstance.h"
#include "ObjectContainer.h"

#include "UI_Text.h"
#include "TextSlot.h"
#include "UI_Cursor.h"
#include "UI_NameTag.h"
#include "SelectPanel.h"
#include "CraftCard.h"

#include "NonPlayer.h"
#include "UI_ItemCard.h"

#include "Helper_Func.h"
#include "IResourceService.h"
#include "Texture.h"
#include "UI_ItemText.h"

#include "Level.h"
#include "ItemSpawner.h"
#include "EventSystem.h"
#include "UI_Transition.h"

#include "AudioSource.h"

CUI_CraftPanel::CUI_CraftPanel()
{
}

CUI_CraftPanel::CUI_CraftPanel(const CUI_CraftPanel& rhs)
	:CUI_Object(rhs)
{
}

HRESULT CUI_CraftPanel::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level","UI_Select.wav","Select");
	Add_Component<CAudioSource>()->Add_Slot("GamePlay_Level","UI_Invalid.wav","Invalid");
	Get_Component<CAudioSource>()->Set_3DAttribute("Select", false);
	Get_Component<CAudioSource>()->Set_SlotVolume("Select", 0.3f);

	Get_Component<CAudioSource>()->Set_3DAttribute("Invalid", false);
	Get_Component<CAudioSource>()->Set_SlotVolume("Invalid", 0.3f);

	return S_OK;
}

HRESULT CUI_CraftPanel::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	Add_Component<CObjectContainer>();

	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_Craft_BackGround.png");
	auto pGradTexture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture("GamePlay_Level", "UI_Gradation.png");

	Get_Component<CSprite2D>()->ChangePass("Gradtion");
	SHADER_PARAM gradParam = { pGradTexture->Get_SRV(),"Texture2D",0 };
	Get_Component<CSprite2D>()->Set_Param("UI_GradationTexture", gradParam);
	Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_Gradation.png");

	return S_OK;
}

void CUI_CraftPanel::Awake()
{
	Read_CraftData();
	Ready_Cards();

	m_bActive = false;
	m_pCraftCard->UI_DeActive(nullptr);
	for (size_t i = 0; i < m_pCards.size(); i++)
	{
		m_pCards[i]->UI_DeActive(nullptr);
	}
	Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	m_pText->DeActive_Text();
}

void CUI_CraftPanel::Priority_Update(_float dt)
{
	Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	if (!m_bActive) return;
	m_PrevIndex = m_NowIndex;
	if (m_eState == Opened) {
		m_pText->Set_Text(m_pCards[m_NowIndex]->Get_Data(), true);
		m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(true);
	}
	else {
		m_pText->DeActive_Text();
		m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(false);
	}

	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
	if (m_eState == Closed)
	{
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_SPACE))
			m_eState = Opened;
	}
	else if (m_eState == Opened) {
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_RIGHT)) {
			Valid_Index(1);
		}
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_LEFT)) {
			Valid_Index(-1);
		}
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_UP)) {
			Valid_Index(-m_Col);
		}
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_DOWN)) {
			Valid_Index(m_Col);
		}
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_SPACE)) {
			m_eState = Selected;
			m_pCraftCard->UI_Active(nullptr);
			m_pCraftCard->Set_Data(m_CraftData[m_pCards[m_NowIndex]->Get_Data()]);
			Get_Component<CAudioSource>()->Play("Select");
		}
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_SHIFT)) {
			m_eState = Closed;
			auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
			nowLevel->Get_LevelObject<CUI_Transition>()->Set_DeActive();
		}
	}
	else if (m_eState == Selected) {
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_SHIFT)) {
			m_eState = Opened;
			m_pCraftCard->UI_DeActive(nullptr);
		}
	}

	if (m_eState == Closed)
		m_fDeactiveTime += dt*3;

	if (m_fDeactiveTime > 1.5f) {
		m_bActive = false;
		UI_DeActive(nullptr);
	}

	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CUI_CraftPanel::Update(_float dt)
{
	if (!m_bActive) return;
	m_pCursor->Set_Pivot(m_pCards[m_NowIndex]->Get_CenterPos(), { xCardSize * 0.5f,yCardSize * 0.2f }, { .5f, .5f });
	
	m_pCards[m_NowIndex]->Hover();

	if(m_NowIndex != m_PrevIndex)
		m_pCards[m_PrevIndex]->UnHover();

	m_pText->Set_CenterPos(m_pCards[m_NowIndex]->Local_CT(0.f - 20.f));
	Get_Component<CObjectContainer>()->UpdateChild(dt);
	Render_Cards();
}

void CUI_CraftPanel::Late_Update(_float dt)
{
	if (!m_bActive) return;
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CUI_CraftPanel::Render_GUI()
{

}

void CUI_CraftPanel::UI_Active(void* pArg)
{
	m_fDeactiveTime = 0.f;
	if (m_eState == Closed) {
		auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
		nowLevel->Get_LevelObject<CUI_Transition>()->Set_Active();
	}
	CRAFT_DATA_DESC* pDesc = static_cast<CRAFT_DATA_DESC*>(pArg);
	m_OnClose = pDesc->OnClose;
	m_InvenData = pDesc->InvenData;

	m_bActive = true;
	m_pText->Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	Get_Component<CSprite2D>()->Set_CompActive(m_bActive);

	Compare_CraftData();

	_uint i = 0;
	for (auto pair : m_CraftData)
	{
		m_pCards[i]->UI_Active(pArg);
		m_pCards[i]->Set_Data(pair.second.RecipeImage, pair.second.isAbleToCraft(), pair.second.Result_Item.ItemName);;
		i++;
	}
}

void CUI_CraftPanel::UI_DeActive(void* pArg)
{
	m_fDeactiveTime = 0.f;
	m_pText->Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	Get_Component<CSprite2D>()->Set_CompActive(m_bActive);
	m_pCraftCard->UI_DeActive(pArg);

	CRAFT_RESULT result = {};
		result.Make_Result(m_SelectedData);

	if (m_OnClose)
		m_OnClose(result);
	m_InvenData.clear();
	m_SelectedData = {};
}

void CUI_CraftPanel::Craft_Selected(CraftData result)
{
	m_SelectedData = result;
	m_pCraftCard->UI_DeActive(nullptr);
	m_eState = Closed;
	auto nowLevel = CGameInstance::GetInstance()->Get_CurrentLevel();
	nowLevel->Get_LevelObject<CUI_Transition>()->Set_DeActive();
}

void CUI_CraftPanel::Ready_Cards()
{
	m_CardCount = m_CraftData.size();
	xCardSize = 512 * 0.3f;
	yCardSize = 749 * 0.3f;
	m_Row = (m_CardCount / m_Col) + (m_CardCount % m_Col == 0 ? 0 : 1);

	auto RenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	RenderTargetDesc CardDesc = {
		"CardTexture" ,
		DXGI_FORMAT_R8G8B8A8_UNORM ,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		_float4(0.f, 0.f, 0.f, 0.f) ,
		xCardSize * 5,
		yCardSize * m_Row
	};

	RenderSys->Create_RenderTarget(CardDesc);
	_float xPadding = xCardSize * 0.5;
	_float yPadding = yCardSize * 0.3;

	_float xStart = 0 - m_fSizeX * 0.5f + xCardSize;
	_float yStart = 0 - m_fSizeY * 0.5f + yCardSize * .5f + yPadding;


	for (size_t i = 0; i < m_Row; i++)
	{
		for (size_t j = 0; j < m_Col; j++)
		{
			_uint index = i * m_Col + j;
			if (index >= m_CardCount)
				continue;

			CUI_Object* pItemCard = Builder::Create_UIObject(
				{ "GamePlay_Level", "GamePlay_GameObject_UI_ItemCardl" })
				.Add_To_Level("GamePlay_Level")
				.Position({
					xStart + (j * (xCardSize + xPadding)),
					yStart + (i * (yCardSize + yPadding)) })
				.Scale({ xCardSize, yCardSize })
				.Rotate(XMConvertToRadians(Helper::Get_Random_Float(-25.f, 25.f)))
				.Build("ItemCard");

			Get_Component<CObjectContainer>()->Add_Child(pItemCard, false);
			CUI_ItemCard* card = dynamic_cast<CUI_ItemCard*>(pItemCard);
			card->Set_Index({ (_float)m_Col,(_float)m_Row}, { (_float)j,(_float)i }, { xCardSize,yCardSize }, { xCardSize * 5.f, yCardSize * m_Row });
			m_pCards.push_back(card);
		}
	}


	/*카드*/
	CUI_Object* pCraftCard = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_CraftCard" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ m_fSizeX * 0.95f,m_fSizeY * 0.95f })
		.Position({ 0, 0 })
		.Build("CraftCard");

	m_pCraftCard = dynamic_cast<CCraftCard*>(pCraftCard);
	m_pCraftCard->Set_Panel(this);
	Get_Component<CObjectContainer>()->Add_Child(m_pCraftCard, false);
	m_pCraftCard->Get_Component<CSprite2D>()->Set_CompActive(true);


	/*커서*/ 
	CUI_Object* pCursor = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Cursor" })
		.Add_To_Level("GamePlay_Level")
		.Scale({ 80,80 })
		.Position({ 0, 0 })
		.Build("Dialcursor");

	m_pCursor = dynamic_cast<CUI_Cursor*>(pCursor);
	Get_Component<CObjectContainer>()->Add_Child(m_pCursor, false);
	m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(true);

	CUI_Object* pObj = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_ItemText" })
		.Add_To_Level("GamePlay_Level")
		.Position({ 0,-55 })
		.Build("Icon");

	m_pText = dynamic_cast<CUI_ItemText*>(pObj);
	Add_Component<CObjectContainer>()->Add_Child(m_pText, false); /*표시 */
	m_pText->Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "UI_ItemText.png");
	m_pText->Get_Component<CSprite2D>()->Set_CompActive(false);
}

void CUI_CraftPanel::Render_Cards()
{
	auto RenderSys = CGameInstance::GetInstance()->Get_RenderSystem();
	RENDER_CUSTOM_COMMAND cmd = {
	"CardTexture",
	[this](ID3D11DeviceContext* pContext)
	{
			for (size_t i = 0; i < m_pCards.size(); i++)
		{
				m_pCards[i]->Render_Card(pContext);
		}
	}};

	RenderSys->Add_RenderCommand(cmd);
};

void CUI_CraftPanel::Compare_CraftData()
{
	for (auto& craftData : m_CraftData)
	{
		craftData.second.Reset();
	}
	for (auto& invenData : m_InvenData)
	{
		for (auto& craftData : m_CraftData)
		{
			if (craftData.second.Has(invenData.first)) {
				craftData.second.Add(invenData.first, invenData.second);
			}
		}
	}
}

_int CUI_CraftPanel::Valid_Index(_int Add)
{
	_int next = m_NowIndex + Add;

	if (next < 0)
		next = 0;

	if (next >= m_CardCount)
		next = m_CardCount - 1;

	m_NowIndex = next;
	return m_NowIndex;
}

HRESULT CUI_CraftPanel::Read_CraftData()
{
	auto ItemSpawner = CGameInstance::GetInstance()->Get_CurrentLevel()->Get_LevelObject<CItemSpawner>();

	if (!ItemSpawner)
		return E_FAIL;

	string filePath = "../../Resources/Data/RecipeData.json";

	ifstream ifs(filePath);
	if (!ifs.is_open()) {
		MessageBoxW(nullptr, L"ItemData 파일을 찾을 수 없습니다.", L"Error", MB_OK);
		return E_FAIL;
	}

	json jScene;

	try {
		ifs >> jScene;
	}
	catch (const json::parse_error& e) {
		MessageBoxA(nullptr, e.what(), "JSON Parse Error", MB_OK);
		return E_FAIL;
	}
	ifs.close();

	m_CraftData.clear();

	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	// JSON 배열 순회
	for (auto& item : jScene)
	{
		try
		{
			CraftData data = {};

			if (item.contains("ItemKey"))
			{
				string ResultKey = item.value("ItemKey", "");
				data.Result_Item = ItemSpawner->Get_ItemData(ResultKey);
			}
			if (item.contains("RecipeImage"))
			{
				data.RecipeImage ="MenuLayout_"+item.value("RecipeImage", "");
			}

			if (item.contains("ResourceItem01"))
			{
				string itemKey = item.value("ResourceItem01", "");
				data.ResourceItem_01 = ItemSpawner->Get_ItemData(itemKey);
			}
			if (item.contains("ResourceItem01_Count"))
			{
				if (item["ResourceItem01_Count"].is_number_integer())
					data.ResourceItemCount_01 = item["ResourceItem01_Count"].get<_int>();
				else if (item["ResourceItem01_Count"].is_string())
					data.ResourceItemCount_01 = std::stoul(item["ResourceItem01_Count"].get<string>());
			}

			if (item.contains("ResourceItem02"))
			{
				string itemKey = item.value("ResourceItem02", "");
				data.ResourceItem_02 = ItemSpawner->Get_ItemData(itemKey);
			}
			if (item.contains("ResourceItem02_Count"))
			{
				if (item["ResourceItem02_Count"].is_number_integer())
					data.ResourceItemCount_02 = item["ResourceItem02_Count"].get<_int>();
				else if (item["ResourceItem02_Count"].is_string())
					data.ResourceItemCount_02 = std::stoul(item["ResourceItem02_Count"].get<string>());
			}

			if (item.contains("ResourceItem03"))
			{
				string itemKey = item.value("ResourceItem03", "");
				data.ResourceItem_03 = ItemSpawner->Get_ItemData(itemKey);
			}
			if (item.contains("ResourceItem03_Count"))
			{
				if (item["ResourceItem03_Count"].is_number_integer())
					data.ResourceItemCount_03 = item["ResourceItem03_Count"].get<_int>();
				else if (item["ResourceItem03_Count"].is_string())
					data.ResourceItemCount_03 = std::stoul(item["ResourceItem03_Count"].get<string>());
			}

			m_CraftData[data.Result_Item.ItemName] = data;
		}
		catch (...)
		{
			continue;
		}
	}
	return S_OK;
}

CUI_CraftPanel* CUI_CraftPanel::Create()
{
	CUI_CraftPanel* instance = new CUI_CraftPanel();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CUI_CraftPanel");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CUI_CraftPanel::Clone(INIT_DESC* pArg)
{
	CUI_CraftPanel* instance = new CUI_CraftPanel(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CUI_CraftPanel");
		Safe_Release(instance);
	}

	return instance;
}

void CUI_CraftPanel::Free()
{
	__super::Free();
}
