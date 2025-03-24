#pragma once
#include "CWeapon.h"
class CSword : public CWeapon
{
public:
	CSword(tagItem _item, int _Point);
	CSword(const CSword& rhs);
	~CSword() override;
public:
	// CWeapon을(를) 통해 상속됨
	void Init() override;
	void Release() override;
	void Render() override;
public:
	int GetLevel() override;
	int GetCostl() override;
	bool Equip() override;
	E_Item GetCategory() override;

private:
	int m_AtkPoint;

	// CWeapon을(를) 통해 상속됨
};


