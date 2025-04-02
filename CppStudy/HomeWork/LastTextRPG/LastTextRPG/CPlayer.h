#pragma once
#include "CObject.h"
class CInventory;
class CEquip;
class CItem;

class CPlayer:public CObject
{
public:
	CPlayer();
	~CPlayer() override;
public :
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;
	
public:
	void SetName();
	void SetClass(tagStatus stat,CLASS _Class);
	void Purchase(CItem* item);

	void ShowStatus();
	void ShowInventory();
	void ShowSkill();
	
	int GetGold() { return m_Stat.iGold; };
	int GetAtkType()override;

	void GetProps(CObject* object);
	CObject* GetCompete();
	void SetCompete(CObject* object);

private:
	wstring ReturnClass(CLASS _class);
	void GetExp(int exp);
	void LevelUp();

private:
	CLASS m_Class;
	CInventory* m_inven;
	CEquip* m_Equip;
	int m_AtkType;
	int m_MaxExp;
	int m_BonusStat;

	CObject* m_compete; //대전 상대
};

