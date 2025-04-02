#pragma once
class CItem;
class CObject;
class CPlayer;
class CDataMgr
{
public:
	CDataMgr();
	~CDataMgr();

public:
	void Initialize();
	void Release();
public:
	void LoadPlayer(CLASS _class, CPlayer* player);
	void LoadMonster();
	void LoadItem();
	vector<CObject*>& GetMonster(int level);
	vector<CItem*>& GetDisposData();
	vector<CItem*>& GetEquipData();

private:
	tagStatus ApplyStat(const vector<wstring>& _dataLine);
	tagItem ApplyInfo(const vector<wstring>& _dataLine);

private:
	vector<vector<CObject*>>m_monsterData;
	vector<CItem*> m_DisposItem;
	vector<CItem*> m_EquipItem;

};

