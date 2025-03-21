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
	CObject** GetMonster();
public :
	void LoadCItem();
	CItem** GetItem();

private:
	CObject** m_MonsterArr;
	CItem** m_ItemArr;
};

