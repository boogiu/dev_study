#pragma once
class CObject;

class CItem
{
public:
	CItem();
	virtual ~CItem();

public:
	virtual void Initialize();
	virtual void Release();
	void Render();
	void RenderSummary();
	int GetCost() { return m_item.m_Cost; }
	wstring& GetName() { return m_item.m_Name; }
	virtual void Active(CObject* obj) = 0;
	virtual void DeActive(CObject* obj) = 0 ;
private:

protected:
	tagItem m_item;
};

