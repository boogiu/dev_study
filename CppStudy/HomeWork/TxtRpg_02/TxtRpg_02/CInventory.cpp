#include "pch.h"
#include "CInventory.h"
#include "Items.h"

CInventory::CInventory()
	:m_Capcity(5), m_Size(0)
{

}

CInventory::CInventory(int Capacity)
	:m_Capcity(Capacity), m_Size(0)
{

}

CInventory::~CInventory()
{

}

void CInventory::Init()
{
}

void CInventory::Render()
{
	for (CItem* item : m_inventory) {
		std::cout << "----------------------" << std::endl;
		item->Render();
	}
	std::cout << "----------------------" << std::endl;

	system("pause");
	int iSelect = CFunction::SafeInt(1, 2);
	std::cout << "1. 확인" << std::endl;
	if (iSelect == 1) {
		return;
	}
}

void CInventory::Release()
{
	for (CItem* item : m_inventory) {
		item->Release();
		SAFE_DELETE(item);
	}
}

bool CInventory::PushItem(CItem* _item)
{
	if (!_item) return false;

	if (m_Capcity == m_Size) {
		std::cout << "인벤토리가 꽉 찼습니다!" << std::endl;
		return false;
	}

	E_Item e_cat = _item->GetCategory();
	//포인터를 받아왔지만, 원본을 복사하는 걸로.
	CItem* item = nullptr;
	switch (e_cat)
	{
	case NONE:
		break;
	case HEALTHPOTION:
	{
		CHealthPotion* tmp = dynamic_cast<CHealthPotion*>(_item); //스태틱 써도 됨
		item = new CHealthPotion(*tmp);
	}
	break;
	case SWORD:
	{
		CSword* tmp = dynamic_cast<CSword*>(_item);
		item = new CSword(*tmp);
	}
	break;
	case ENDITEM:
		break;
	default:
		break;
	}
	if (item) {
		m_inventory.push_back(item);
		m_Size++;
		return true;
	}
	return false;
}
void CInventory::PopItem(int index)
{
}

void CInventory::UseItem(int index)
{
}
