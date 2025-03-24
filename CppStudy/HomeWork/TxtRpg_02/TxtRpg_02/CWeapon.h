#pragma once
#include "CItem.h"
class CWeapon:public CItem
{
public:
	CWeapon();
	~CWeapon() override;
public:
	virtual bool Equip()=0;
};

