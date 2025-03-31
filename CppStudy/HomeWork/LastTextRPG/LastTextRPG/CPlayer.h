#pragma once
#include "CObject.h"
class CInventory;
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
	void ShowStatus();
	void ShowInventory();
	void ShowInventory(CObject* compete);
	void ShowSkill();
	int GetGold() { return m_Stat.iGold; };
	int GetAtkType()override;
	void Purchase(CItem *item);
	void GetProps(CObject* object);
	void UseSlot(int index, CObject*target);
private:
	wstring ReturnClass(CLASS _class);
	void GetExp(int exp);
	void LevelUp();

private:
	CLASS m_Class;
	CInventory* m_inven;
	int m_AtkType;
	int m_MaxExp;
	int m_BonusStat;
};

