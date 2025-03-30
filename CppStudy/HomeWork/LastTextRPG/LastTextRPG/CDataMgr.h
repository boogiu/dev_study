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
	vector<CObject*>& GetMonster(int level);
private:
	tagStatus ApplyStat(const vector<wstring>& _dataLine);
private:
	vector<vector<CObject*>>m_monsterData;
	vector<CItem*> m_itemData;
};

