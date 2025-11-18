#include "Client_Defines.h"
#include "Player_Inventory.h"
#include "Sprite2D.h"
#include "GameInstance.h"
#include "ObjectContainer.h"
#include "UI_InvenSlot.h"
#include "UI_Cursor.h"
#include "SelectPanel.h"
#include "Player.h"

CPlayer_Inventory::CPlayer_Inventory()
{
}

CPlayer_Inventory::CPlayer_Inventory(const CPlayer_Inventory& rhs)
	:CUI_Object(rhs)
{
}

CPlayer_Inventory::~CPlayer_Inventory()
{
}

HRESULT CPlayer_Inventory::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CPlayer_Inventory::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Add_Component<CObjectContainer>();

	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture(G_GlobalLevelKey, "UI_InventoeyPanel.png");
	Get_Component<CSprite2D>()->Set_CompActive(false);

	m_eState = Closed;
	m_fLocalX = m_vOpenPos.x; m_fLocalY = m_vOpenPos.y;
	for (size_t i = 0; i < 20; i++)
	{
		CUI_Object* pUI =Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_InvenSlot" })
			.Add_To_Level("GamePlay_Level")
			.Build("Slot");
		Get_Component<CObjectContainer>()->Add_Child(pUI, false);
		m_pSlots.push_back(dynamic_cast<CUI_InvenSlot*>(pUI));
	}

	CUI_Object* pUI = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_Cursor" })
		.Add_To_Level("GamePlay_Level")
		.Scale({50,50 })
		.Position({0,0})
		.Build("Cursor");

		CUI_Object* pSelectUI = Builder::Create_UIObject({ "GamePlay_Level", "GamePlay_GameObject_UI_SelectPanel" })
		.Add_To_Level("GamePlay_Level")
		.Scale({0,0 })
		.Position({180,120})
		.Build("Select");

	Get_Component<CObjectContainer>()->Add_Child(pUI, false);
	Get_Component<CObjectContainer>()->Add_Child(pSelectUI, false);

	m_pCursor= dynamic_cast<CUI_Cursor*>(pUI);
	m_pSelectPanel = dynamic_cast<CSelectPanel*>(pSelectUI);

	return S_OK;
}

void CPlayer_Inventory::Priority_Update(_float dt)
{
	if (m_eState != Closing) {
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_SPACE)) {
			if (m_eState == Opened)
				m_eState = Selected;
		}
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_SHIFT)) {
			if (m_eState == Selected) {
				m_pSelectPanel->DeActive();
				m_eState = Opened;
			}
		}
	}
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CPlayer_Inventory::Update(_float dt)
{
	switch (m_eState)
	{
	case Client::CPlayer_Inventory::Openning:
		Openning_Inven(dt);
		break;
	case Client::CPlayer_Inventory::Opened:
		Pointing_Item(dt);
		Get_Component<CObjectContainer>()->UpdateChild(dt);
		break;
	case Client::CPlayer_Inventory::Selected:
		Select_Item(dt);
		Get_Component<CObjectContainer>()->UpdateChild(dt);
		break;
	case Client::CPlayer_Inventory::Closing:
		Closing_Inven(dt);
		break;
	case Client::CPlayer_Inventory::Closed:
		break;
	default:
		break;
	}
}

void CPlayer_Inventory::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CPlayer_Inventory::Render_GUI()
{
	__super::Render_GUI();
}

void CPlayer_Inventory::Set_Player(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

void CPlayer_Inventory::Open_Inventory()
{
	Get_Component<CSprite2D>()->Set_CompActive(true);
	m_eState = Openning;
}

void CPlayer_Inventory::Close_Inventory()
{
	m_eState = Closing;
}

HRESULT CPlayer_Inventory::Add_ItemToInventory(ITEM_DATA_DESC desc)
{
	if (desc.TypeTag == itemType::None)
	{
		return E_FAIL;
	}
	else {
		for (size_t i = 0; i < m_pSlots.size(); i++)
		{
			if (m_pSlots[i]->isAbleToContain(desc)) {
				m_pSlots[i]->Add_Data(desc);
				return S_OK;
			}
		}
	}
	return E_FAIL;
}

HRESULT CPlayer_Inventory::PullOut_Item(_int Slot)
{
	if (m_pSlots[Slot]->isItemFilled()) {
		m_pSlots[Slot]->PullOut_Data();
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Inventory::PullOut_ToOtherSlot(_int Slot)
{
	ITEM_DATA_DESC desc=m_pSlots[Slot]->Get_Data();

	for (size_t i = 0; i < m_pSlots.size(); i++)
	{
		if (i == Slot) continue;

		if (m_pSlots[i]->isAbleToContain(desc)) {
			m_pSlots[i]->Add_Data(desc);
			m_pSlots[Slot]->PullOut_Data();
			return S_OK;
		}
	}
	return E_FAIL;
}

HRESULT CPlayer_Inventory::PullOut_Item(ITEM_DATA_DESC data, _uint Count)
{
	_uint RemovedCount = 0;

	for (size_t i = 0; i < m_pSlots.size(); i++)
	{
		if (RemovedCount == Count)
			return S_OK;

		ITEM_DATA_DESC item = m_pSlots[i]->Get_Data();
		if (item.FileName != data.FileName)
			continue;

		//카운트 남아있고, 삭제할게 남아있으면
		while (m_pSlots[i]->Get_Count() > 0 && RemovedCount < Count)
		{
			m_pSlots[i]->PullOut_Data();
			RemovedCount++;
		}
	}

	return S_OK;
}


unordered_map<wstring, _uint> CPlayer_Inventory::Get_All_InventoryData()
{
	ITEM_DATA_DESC desc;
	//vector<pair<wstring, _uint>> InvenData = {};
	unordered_map<wstring, _uint> InvenData;

	for (size_t i = 0; i < m_pSlots.size(); i++)
	{
		wstring key = m_pSlots[i]->Get_Data().ItemName;
		_uint count = m_pSlots[i]->Get_Count();

		auto iter = InvenData.emplace(key, count);

		if (!iter.second) {
			iter.first->second += count;
		}
	}

	return InvenData;
}

void CPlayer_Inventory::Batch_Slots()
{
	const _uint slotColumn = 10;
	const _float amplitude = -23.f;
	const _float width = m_fSizeX;
	const _float spacing = 35;

	for (size_t i = 0; i < slotColumn; i++)
	{
		float x = static_cast<float>(i) / (slotColumn - 1);
		float y = sinf(x * XM_PI);/*0부터 파이까지*/
		float xOffset = spacing * (i - 5.f) + 20.f;
		m_pSlots[i]->Set_CenterPos({ xOffset,amplitude * y - 10 });
		m_pSlots[i]->Set_Size(_vector{ 15, 15 });
		m_pSlots[i]->Active();
	}

	for (size_t i =0; i < slotColumn; i++)
	{
		//사인 그래프 형태로
		float x = static_cast<float>(i) / (slotColumn - 1);
		float y = sinf(x * XM_PI);/*0부터 파이까지*/
		float xOffset = spacing * (i - 5.f) + 20.f;

		m_pSlots[i+slotColumn]->Set_CenterPos({ xOffset,amplitude * y + 40 });
		m_pSlots[i+slotColumn]->Set_Size(_vector{ 15, 15 });
		m_pSlots[i + slotColumn]->Get_Component<CSprite2D>()->Set_CompActive(true);
	}
	m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(true);
}

void CPlayer_Inventory::DeActive_Slots()
{
	for (auto& slot : m_pSlots)
	{
		slot->DeActive();
	}
	m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(false);

}

void CPlayer_Inventory::Openning_Inven(_float dt)
{
	if (m_eState != Openning) return;

	m_vTimer.x += dt * 3.f;
	float t = clamp(m_vTimer.x, 0.f, 1.f);

	_vector pos;
	_vector size;
	_float2 CurPos = { m_fLocalX,m_fLocalY};

	if (t < 0.5f) {
		pos = XMVectorLerp(XMLoadFloat2(&m_vClosePos), XMLoadFloat2(&m_vPointPos), t);
	}
	else {
		pos = XMVectorLerp(XMLoadFloat2(&CurPos), XMLoadFloat2(&m_vOpenPos), dt*3 );
	}

	size = XMVectorLerp(XMLoadFloat2(&m_vCloseSize), XMLoadFloat2(&m_vOpenSize), t);

	_float2 vPos, vSize;
	XMStoreFloat2(&vPos, pos);
	XMStoreFloat2(&vSize, size);

	m_fSizeX = vSize.x;
	m_fSizeY = vSize.y;
	m_fLocalX = vPos.x;
	m_fLocalY = vPos.y;

	if (m_vTimer.x > 1.f) {
		Batch_Slots();
		m_eState = Opened;
		m_vTimer.x = 0;
	}
}

void CPlayer_Inventory::Closing_Inven(_float dt)
{
	DeActive_Slots();
	m_pSelectPanel->DeActive();

	m_vTimer.x += dt * 8;
	_vector pos = XMVectorLerp(XMLoadFloat2(&m_vOpenPos), XMLoadFloat2(&m_vClosePos), m_vTimer.x);
	_vector size = XMVectorLerp(XMLoadFloat2(&m_vOpenSize), XMLoadFloat2(&m_vCloseSize), m_vTimer.x);

	_float2 vPos;
	_float2 vSize;

	XMStoreFloat2(&vPos, pos);
	XMStoreFloat2(&vSize, size);

	m_fSizeX = vSize.x;
	m_fSizeY = vSize.y;

	m_fLocalX = vPos.x;
	m_fLocalY = vPos.y;

	if (m_vTimer.x > 1.f) {
		m_vTimer.x = 0;
		m_eState = Closed;
		Get_Component<CSprite2D>()->Set_CompActive(false);
	}
}
void CPlayer_Inventory::Pointing_Item(_float dt)
{
	m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(true);
	auto InputDev = CGameInstance::GetInstance()->Get_InputDev();
	bool moved = false; // 이번 프레임에 인덱스가 변했는가

	if (InputDev->Key_Tap(VK_RIGHT)) {
		m_pSlots[nowIndex]->isHoverOut();
		nowIndex = clamp(nowIndex + 1, 0, 19);
		moved = true;
	}
	else if (InputDev->Key_Tap(VK_LEFT)) {
		m_pSlots[nowIndex]->isHoverOut();
		nowIndex = clamp(nowIndex - 1, 0, 19);
		moved = true;
	}
	else if (InputDev->Key_Tap(VK_DOWN)) {
		if (nowIndex + 10 < 20) {
			m_pSlots[nowIndex]->isHoverOut();
			nowIndex += 10;
			moved = true;
		}
	}
	else if (InputDev->Key_Tap(VK_UP)) {
		if (nowIndex - 10 >= 0) {
			m_pSlots[nowIndex]->isHoverOut();
			nowIndex -= 10;
			moved = true;
		}
	}

	if (moved) {
	}
	m_pSlots[nowIndex]->isHovered();
	m_pCursor->Set_Pivot(m_pSlots[nowIndex]->Get_CenterPos());
}


void CPlayer_Inventory::Select_Item(_float dt)
{
	if(!m_pSlots[nowIndex]->isItemFilled()){
		m_eState = Opened;
		return;
	}
	m_pCursor->Get_Component<CSprite2D>()->Set_CompActive(false);
	m_pSlots[nowIndex]->isHovered();
	m_pSelectPanel->Get_Component<CSprite2D>()->Set_CompActive(true);

	ITEM_DATA_DESC Data = dynamic_cast<CUI_InvenSlot*>(m_pSlots[nowIndex])->Get_Data();
	auto vector = Switch_ItemSelect(Data.TypeTag, m_pSlots[nowIndex]->Get_Count());
	m_pSelectPanel->Set_Selecte(vector,dt);
	m_pSelectPanel->Active();
	_int selectedAction = m_pSelectPanel->Check_Select();

	if (selectedAction != -1) {
		m_pPlayer->Set_InvenEvent(Data, nowIndex, vector[selectedAction]);
		//DeActive_Slots();
		m_pSelectPanel->DeActive();
		if(m_eState != Closing)
			m_eState = Opened;
	}
}

vector<wstring> CPlayer_Inventory::Switch_ItemSelect(itemType type, _uint count)
{
	vector<wstring>SelectScript;

	switch (type)
	{
	case itemType::None:
		return vector<wstring>();

	case itemType::Fruit:
		SelectScript= { L"근처에 두기",L"1개 먹기"};
		if (count > 1)
			SelectScript.push_back(L"1개 꺼내기");
			break;

	case itemType::Ore:
		SelectScript = { L"근처에 두기" };
		if (count > 1)
			SelectScript.push_back(L"1개 꺼내기");
		break;

	case itemType::Insect:
		SelectScript = { L"근처에 두기", L"풀어주기"};
		break;
	case itemType::Axe:
	case itemType::Scoop:
	case itemType::Net:
	case itemType::FishingRod:
		SelectScript = { L"들기" , L"배치하기", L"근처에 두기" };
		if (m_pPlayer->Get_ItemPacket().CurItem.TypeTag == type)
			SelectScript[0] = L"장착 해제하기";
		break;
	case itemType::Represent:
	case itemType::Furniture:
		SelectScript = {L"배치하기", L"근처에 두기" };
			break;
	default:
		break;
	}
	return SelectScript;
}


CPlayer_Inventory* CPlayer_Inventory::Create()
{
	CPlayer_Inventory* instance = new CPlayer_Inventory();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Object Create Failed : CPlayer_Inventory");
		Safe_Release(instance);
	}
	return instance;
}

CGameObject* CPlayer_Inventory::Clone(INIT_DESC* pArg)
{
	CPlayer_Inventory* instance = new CPlayer_Inventory(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CPlayer_Inventory");
		Safe_Release(instance);
	}

	return instance;
}

void CPlayer_Inventory::Free()
{
	__super::Free();
}