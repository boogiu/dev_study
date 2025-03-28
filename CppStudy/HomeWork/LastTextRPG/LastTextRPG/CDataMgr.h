#pragma once
class CItem;
class CObject;

class CDataMgr
{
public:
	CDataMgr();
	~CDataMgr();

public:
	void Initialize();
	void Release();
public:
	void LoadPlayer();
	void LoadMonster();
	void LoadItem();

private:
	vector<CObject*> m_monsterData;
	vector<CItem*> m_itemData;
};

