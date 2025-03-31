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
	
	void ActiveItem(CObject* obj);
	
	void PushItem(CItem* item);
	void PopItem();
	
	CItem* GetItem(){ return m_Item; }
public :
	bool m_Empty;

private :
	CItem* m_Item;
	int m_Count;
};

