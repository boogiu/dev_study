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
	std::cout << "1. 확인\t 2. 아이템 사용하기" << std::endl;
	int iSelect = CFunction::SafeInt(1, 2);
	if (iSelect == 1) {
		return;
	}
}

void CInventory::Release()
{
	//여기서 삭제하면 안된다.
}

bool CInventory::PushItem(CItem* _item)
{
	if (!_item) return false;

	if (m_Capcity == m_Size) {
		std::cout << "인벤토리가 꽉 찼습니다!" << std::endl;
		return false;
	}


	CItem* item = _item;
	
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
