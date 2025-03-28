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
	for (int i = 0; i < m_Size; ++i) {
		std::cout << "----------------------" << std::endl;
		std::cout <<i +1<< "번. ";
		m_inventory[i]->Render();
	}
	std::cout << "----------------------" << std::endl;
	std::cout << "1. 확인\t 2. 아이템 사용하기" << std::endl;
	int iSelect = CFunction::SafeInt(1, 2);
	if (iSelect == 1) {
		return;
	}
	else {
		iSelect = 0;
		std::cout << "사용할 아이템의 번호" << std::endl;
		iSelect = CFunction::SafeInt(1, m_Size);

		// m_inventory[iSelect - 1];
		/*
			고민: 상속 관계를 어떻게 할지
			1) Activate라는 공통 함수를 상속 받아서 EQUI별로 한다.
			2) dynamicCast를 활용한다. 널포인터 아닌지로 switch받아서
			3) 인터페이스를 통해 활용한다. (IEquipable / iConsumable)
			4) 아니면, 클래스 안에 사용 타입을 구분해도 괜찮겟네. (이게 제일 간단하고 직관적일 수도.) 
			=> Activate / deActivate 나누어서 enum에따라 Activate 진행 : 
			Equipable의 경우 장비창으로 이동. 거기서 나오면 deActivate 및 다시 인벤토리로 이동.
		*/
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
