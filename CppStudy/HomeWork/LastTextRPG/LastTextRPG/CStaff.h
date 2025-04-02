#pragma once
#include "CEquipable.h"

class CStaff : public CEquipable
{
public:
	CStaff(tagItem item);
	virtual ~CStaff();

public:
	void Initialize()override;
	void Release()override;
	void Equip() override;

	// CEquipable을(를) 통해 상속됨
	void DeEquip() override;
};