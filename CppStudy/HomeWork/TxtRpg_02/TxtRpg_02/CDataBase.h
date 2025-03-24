#pragma once

class CPlayer;
class CObject;
class CItem;

class CDataBase
{
public :
	CDataBase();
	~CDataBase();

public:
	void Init();
	void Release();

public:
	void LoadPlayer(CPlayer* pPlayer);
	void SavePlayer(CPlayer* pPlayer);
	void SetClass(CPlayer* pPlayer);

public :
	void LoadMonster();
	CObject* GetRandMonster(int _level);

public :
	void LoadCItem();
	const std::vector<CItem*>&  GetItem() const;

private:
	std::vector<CObject*> m_monsterVec;
	std::vector<CItem*> m_ItemArr;
};

