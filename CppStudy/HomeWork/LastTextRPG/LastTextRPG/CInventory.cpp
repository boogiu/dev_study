#include "pch.h"
#include "CInventory.h"
#include "CSlot.h"
#include "Items.h"

CInventory::CInventory()
	:m_SlotCount(5)
{

}

CInventory::~CInventory()
{
}

void CInventory::Initialize()
{
	for (int i = 0; i < m_SlotCount; i++) {
		m_vecSlot.push_back(new CSlot);
	}
}

void CInventory::Release()
{
	for (int i = 0; i < m_SlotCount; i++) {
		m_vecSlot[i]->Release();
		SAFE_DELETE(m_vecSlot[i]);
	}
}

void CInventory::ShowInven()
{
	for (int i = 0; i < m_vecSlot.size(); ++i) {
		wcout << i + 1 << L"번 슬롯 : ";
		m_vecSlot[i]->Render();
	}

}

bool CInventory::PushItem(CItem* _item)
{
	for (int i = 0; i < m_vecSlot.size(); ++i) {
		if (dynamic_cast<CDispoable*>(_item)) {
			if (m_vecSlot[i]->m_Empty || m_vecSlot[i]->GetItem() == _item) {
				m_vecSlot[i]->PushItem(_item);
				wcout << "구입 성공!" << endl;
				return true;
			}
		}
		//장비템 이면 새로운 슬롯에
		else {
			if (m_vecSlot[i]->m_Empty) {
				m_vecSlot[i]->PushItem(_item);
				wcout << "구입 성공!" << endl;
				return true;
			}
		}
	}
	wcout << "구입 실패!" << endl;
	return false;
}

	void CInventory::PopItem(int _index)
	{
		if (!m_vecSlot[_index]->m_Empty) {
			m_vecSlot[_index]->PopItem();
		}
	}

	bool CInventory::UseSlot(int index, CObject* target)
	{
		if (!m_vecSlot[index]->m_Empty) {
			m_vecSlot[index]->ActiveItem(target);
			return true;
		}
		return false;
	}

	CItem* CInventory::CheckItemPtr(int index)
	{
		return m_vecSlot[index]->GetItem();
	}

