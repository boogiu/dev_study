#pragma once
class CItem;
class CObject
	;
class CSlot
{
public :
	CSlot();
	~CSlot();

public:
	void Initialize();
	void Render();
	void Release();

public:
	bool PushItem(CItem* item);
	void PopItem();
	void ActiveItem();
	void DeActiveItem();
	ITEM_TYPE GetItemType();
	bool Equipable();
	void SwapEquip(CSlot& slot);
	int GetCount() { return m_Count; }
public :
	bool m_Empty;

private :
	CItem* m_Item; //데이터 베이스에서 삭제
	int m_Count;
};

