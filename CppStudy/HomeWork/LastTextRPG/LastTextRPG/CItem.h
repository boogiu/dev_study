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

public:
	void Render();
	void RenderSummary();
	int GetCost() { return m_item.m_Cost; }
	std::wstring& GetName() { return m_item.m_Name; }
	ITEM_TYPE type;
private:

protected:
	tagItem m_item;
};

