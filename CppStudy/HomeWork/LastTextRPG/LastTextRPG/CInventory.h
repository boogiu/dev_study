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
	void ShowItems();
	void UseSlot(CSlot* slot);
	void ShowEquips();

public:
	bool PushItem(CItem* _item);
	int GetSlotCount() { return m_SlotCount; }

private : 
	int m_SlotCount;
	std::vector<CSlot*> m_vecItem;
	unordered_map<ITEM_TYPE, CSlot*> m_equipSlot;
};

