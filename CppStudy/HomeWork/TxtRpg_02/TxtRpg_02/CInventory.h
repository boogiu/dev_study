#pragma once

class CItem;
class CInventory
{
public: 
	CInventory();
	CInventory(int Capacity);

	~CInventory();
public:
	void Init();
	void Render();
	void Release();
public:
	bool PushItem(CItem* _item);
	void PopItem(int index);

	void UseItem(int index);
private:
	int m_Capcity;
	int m_Size;
	std::vector<CItem*> m_inventory;
};

