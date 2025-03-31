#pragma once
class CSlot;
class CItem;
class CObject;

class CInventory
{
public: 
	CInventory();
	~CInventory();
public:
	void Initialize();
	void Release();
	void ShowInven();
	
	bool PushItem(CItem* _item);
	void PopItem(int _index);

	bool UseSlot(int index, CObject* target);
	
	CItem* CheckItemPtr(int index);
	
	int GetSlotCount() { return m_SlotCount; }

private : 
	int m_SlotCount;
	std::vector<CSlot*> m_vecSlot;
};

