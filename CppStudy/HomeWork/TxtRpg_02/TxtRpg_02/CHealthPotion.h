#pragma once
#include "CPotion.h"
class CPlayer;
class CHealthPotion : public CPotion
{
public:
	CHealthPotion();
	~CHealthPotion() override;

public:
	// CPotion을(를) 통해 상속됨
	void Init() override;
	void Release() override;
	void Activate(CPlayer*_player) override;

public :
	int GetCost() {return m_ItemInfo.m_iCost;};
private:
	int m_HealPoint;
};

