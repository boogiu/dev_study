#include "pch.h"
#include "CInventory.h"
#include "CSlot.h"
#include "Items.h"

CInventory::CInventory()
	: m_SlotCount(5)
{

}

CInventory::~CInventory()
{
}

void CInventory::Initialize()
{
	for (int i = 0; i < m_SlotCount; i++) {
		m_vecItem.push_back(new CSlot());
	}
	m_equipSlot.insert({ ITEM_TYPE::HELMET, new CSlot() });
	m_equipSlot.insert({ ITEM_TYPE::LEFT, new CSlot() });
	m_equipSlot.insert({ ITEM_TYPE::RIGHT, new CSlot() });
	m_equipSlot.insert({ ITEM_TYPE::ARMOR, new CSlot() });
}

void CInventory::Release()
{
	for (int i = 0; i < m_SlotCount; i++) {
		m_vecItem[i]->Release();
		SAFE_DELETE(m_vecItem[i]);
	}

	unordered_map<ITEM_TYPE, CSlot*>::iterator iter = m_equipSlot.begin();
	for (;iter != m_equipSlot.end(); ++iter) {
		SAFE_DELETE(iter->second);
	}
}
void CInventory::ShowInven() {
	while (true)
	{
		wcout << L"1.  인벤토리 목록 보기 " << endl;
		wcout << L"2.  착용 중인 장비 보기 " << endl;
		wcout << L"3.  나가기 " << endl;

		int iSelect = CInput::SafeInt(1, 3);
		if (iSelect == 3) {
			return;
		}

		else if (iSelect == 1) {
			ShowItems();
		}
		else if (iSelect == 2) {
			ShowEquips();
		}
		system("cls");
	}

}

void CInventory::ShowItems()
{
	system("cls");
	int iSelect = 0;
	while (true)
	{
		for (int i = 0; i < m_vecItem.size(); ++i) {
			if (i == iSelect) {
				wcout << L"▶ ";
			}
			wcout << i + 1 << L"번 슬롯 : ";
			m_vecItem[i]->Render();
			if (m_vecItem[i]->GetCount()) {
				wcout << m_vecItem[i]->GetCount() << L" 개 " << endl;
			}
			else {
				wcout << endl;
			}
		}

		wcout << L"ENTER : 사용\t ESC:나가기"<<endl;

		KEY tmpKey = CInput::GetKey();
		switch (tmpKey)
		{
		case KEY::UP:
			iSelect -= 1;
			break;
		case KEY::DOWN:
			iSelect += 1;
			break;
		case KEY::ENTER:
			UseSlot(m_vecItem[iSelect]);
			break;
		case KEY::ESC:
			return;
		default:
			break;
		}

		if (iSelect < 0) {
			iSelect = m_vecItem.size() - 1;
		}
		if (iSelect > m_vecItem.size() - 1) {
			iSelect = 0;
		}
		system("cls");
	}
}

void CInventory::ShowEquips()
{
	system("cls");
	unordered_map<ITEM_TYPE, CSlot*>::iterator iterTmp = m_equipSlot.begin();
	while (true)
	{
		unordered_map<ITEM_TYPE, CSlot*>::iterator iter = m_equipSlot.begin();
		for (;iter != m_equipSlot.end(); ++iter) {
			if (iter == iterTmp) {
				wcout << L"▶ ";
			}
			wcout << L"슬롯 : ";
			iter->second->Render();
		}

		wcout << L"ENTER : 사용\t ESC:나가기" << endl;

		KEY tmpKey = CInput::GetKey();
		switch (tmpKey)
		{
		case KEY::UP:
			if (iterTmp == m_equipSlot.begin()) {
				iterTmp = m_equipSlot.end();
			}				
			iterTmp--;
			break;
		case KEY::DOWN:
			iterTmp++;
			if (iterTmp == m_equipSlot.end()) {
				iterTmp = m_equipSlot.begin();
			}
			break;
		case KEY::ENTER:
			UseSlot(iterTmp->second);
			break;
		case KEY::ESC:
			return;
		default:
			break;
		}
		system("cls");
	}
}

bool CInventory::PushItem(CItem* _item)
{
	for (int i = 0; i < m_SlotCount; ++i) { //인벤토리가 비어있다면
		if (m_vecItem[i]->PushItem(_item)) {
			return true;
		}
	}
	//인벤토리가 비어있지 않고
	if (dynamic_cast<CEquipable*>(_item)) {
		if (m_equipSlot[_item->type]->PushItem(_item)) { //장비 아이템이며 해당 장비칸이 비어있다면
				m_equipSlot[_item->type]->ActiveItem(); //착용 후에 Active
			return true;
		}
	}

	return false;
}


void CInventory::UseSlot(CSlot* slot)
{
	if (slot->m_Empty) { return; }

	wcout << L"1. 사용하기 \t 2. 전부 버리기" << endl;
	int iSelct = CInput::SafeInt(1, 2);

	if (iSelct == 2) {
		slot->PopItem();
	}
	else {
		if (slot->Equipable()) { //장비템이라면
			slot->SwapEquip(*(m_equipSlot.at(slot->GetItemType())));
		}
		else {
			slot->ActiveItem();
		}
	}
}
