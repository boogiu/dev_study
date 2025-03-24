#pragma once
#include "CPotion.h"
class CPlayer;
class CHealthPotion : public CPotion
{
public:
	CHealthPotion(tagItem _Info,int _Point);
	CHealthPotion(const CHealthPotion& rhs);
	~CHealthPotion() override;

public:
	// CPotion을(를) 통해 상속됨
	void Init() override;
	void Release() override;
	void Activate(CPlayer*_player) override;
	void Render() override;

public :
	int GetLevel() override;
	int GetCost() {return m_ItemInfo.m_iCost;};
	int GetCostl() override;
	E_Item GetCategory() override;

private:
	int m_HealPoint;

};

