#pragma once
class CSlot;
class CItem;
class CObject;

class CEquip
{
public:
	CEquip();
	~CEquip();
public:
	void Initialize();
	void Release();
	void ShowInven();
public:
	bool PushItem(CItem* _item);
	void PopItem(ITEM_TYPE _type);

private:
};

